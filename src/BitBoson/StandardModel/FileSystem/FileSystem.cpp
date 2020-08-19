/* This file is part of standard-model.
 *
 * Copyright (c) BitBoson
 *
 * standard-model is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * standard-model is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with standard-model.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Written by:
 *     - Tyler Parcell <OriginLegend>
 */

#include <fstream>
#include <boost/filesystem/operations.hpp>
#include <BitBoson/StandardModel/Utils/Utils.h>
#include <BitBoson/StandardModel/Crypto/Crypto.h>
#include <BitBoson/StandardModel/FileSystem/FileSystem.h>

using namespace BitBoson::StandardModel;

/**
 * Constructor used to setup the FileSystem object on the provided directory
 *
 * @param directory String representing the file-system object
 */
FileSystem::FileSystem(const std::string& directory)
{

    // Setup the full path reference
    _fullPath = directory;
}

/**
 * Static function used to get a temporary directory FileSystem instance
 * NOTE: This is a managed directory which will be deleted on its own
 *       You can (and it is recommended you do) delete it on your own though
 *
 * @param prefix String representing the desired prefix for the random directory
 * @return FileSystem representing the temporary directory
 */
FileSystem FileSystem::getTemporaryDir(const std::string& prefix)
{

    // Continuously try to create the temporary directory
    // retrying on any clashes
    FileSystem fileSystem;
    do
    {

        // Actually get the temporary directory
        // TODO - Make more cross-platform compatible (i.e. windows)
        auto tmpDir = boost::filesystem::temp_directory_path().string();
        tmpDir += ("/" + prefix + Crypto::getRandomSha256());

        // Create the FileSystem object instance on the temporary directory
        fileSystem = FileSystem(tmpDir);
    }

    // Retry on temporary directory creation on clashes/conflicts
    while (fileSystem.exists());

    // Actually create the temporary directory and return it
    fileSystem.createDir();
    return fileSystem;
}

/**
 * Function used to get the full path string of the FilesSystem instance
 *
 * @return String representing the full path string of the FilesSystem instance
 */
std::string FileSystem::getFullPath() const
{

    // Get and return the setup full path
    return _fullPath;
}

/**
 * Function used to check whether the setup FileSystem instance is a directory
 *
 * @return Boolean indicating whether the setup FileSystem instance is a directory
 */
bool FileSystem::isDirectory() const
{

    // Return whether the path is a directory or not
    return (!_fullPath.empty()) && boost::filesystem::is_directory(_fullPath);
}

/**
 * Function used to check whether the setup FileSystem instance is a directory
 *
 * @return Boolean indicating whether the setup FileSystem instance is a directory
 */
bool FileSystem::isFile() const
{

    // Return whether the path is a file or not
    return (!_fullPath.empty()) && (!boost::filesystem::is_directory(_fullPath));
}

/**
 * Function used to check whether the setup FileSystem instance exists
 * NOTE: This will be true if the FileSystem is either a directory or a file
 *
 * @return Boolean indicating whether the setup FileSystem instance exists
 */
bool FileSystem::exists() const
{

    // Return whether the file/directory exists or not
    return boost::filesystem::exists(_fullPath);
}

/**
 * Function used to get the specified child directory/file of the FileSystem instance
 *
 * @param child String representing the child object to create the new instance on
 * @return FileSystem representing the child directory/file of the FileSystem instance
 */
FileSystem FileSystem::getChild(const std::string& child) const
{

    // Create a FileSystem object on with the provided child
    // TODO - Make more cross-platform compatible (i.e. windows)
    return FileSystem(_fullPath + '/' + child);
}

/**
 * Function used to remove the instance's FileSystem directory
 * NOTE: This will not remove the instance if it is a file not a directory
 *
 * @return Boolean indicating whether the operation was successful or not
 */
bool FileSystem::removeDir()
{

    // Create a return flag to indicate the status of the operation
    bool retFlag = false;

    // Check if the data exists (if not we don't need to do anything)
    if (boost::filesystem::exists(_fullPath) && isDirectory())
    {

        // Delete the directory
        boost::filesystem::remove_all(_fullPath);

        // Indicate that the operation was successful
        retFlag = true;
    }

    // Return the return flag
    return retFlag;
}

/**
 * Function used to remove the instance's FileSystem file
 * NOTE: This will not remove the instance if it is a directory not a file
 *
 * @return Boolean indicating whether the operation was successful or not
 */
bool FileSystem::removeFile()
{

    // Create a return flag to indicate the status of the operation
    bool retFlag = false;

    // Check if the data exists (if not we don't need to do anything)
    if (boost::filesystem::exists(_fullPath) && isFile())
    {

        // Delete the file
        boost::filesystem::remove_all(_fullPath);

        // Indicate that the operation was successful
        retFlag = true;
    }

    // Return the return flag
    return retFlag;
}

/**
 * Function used to create instance's FileSystem as a directory
 *
 * @return Boolean indicating whether the operation was successful or not
 */
bool FileSystem::createDir()
{

    // Create the return flag
    bool retFlag = false;

    // Only try to create the directory if it doesn't already exist
    if (!exists())
    {
        boost::filesystem::create_directories(_fullPath);
        retFlag = boost::filesystem::exists(_fullPath);
    }

    // Return the return flag
    return retFlag;
}

/**
 * Function used to rename the current instance's reference (file or directory)
 *
 * @param newName String representing the instance's new reference name
 * @return Boolean indicating whether the operation was successful or not
 */
bool FileSystem::rename(const std::string& newName)
{

    // Create a return flag
    bool retFlag = false;

    // Obtain the parent directory name for the item to be renamed
    auto splitPath = Utils::splitStringByDelimiter(_fullPath, "/");
    auto parentPath = _fullPath.substr(0, _fullPath.size() - splitPath.back().size() - 1);
    auto parentFileSystem = FileSystem(parentPath);

    // Attempt to rename the directory/file if it exists
    auto newFileSystem = parentFileSystem.getChild(newName);
    if (exists() && !newFileSystem.exists())
        boost::filesystem::rename(_fullPath, newFileSystem.getFullPath());

    // Verify that the new child exists and the old one does not
    retFlag = (!parentFileSystem.getChild(splitPath.back()).exists()) && newFileSystem.exists();

    // If the operation was successful, then re-assign the full path reference
    if (retFlag)
        _fullPath = newFileSystem.getFullPath();

    // Return the return flag
    return retFlag;
}

/**
 * Function used to read-in (stream) the current object as a file
 * NOTE: This will return null if the item is not a file
 *
 * @param bufferSize Long representing the buffer size to use when reading
 * @return Generator of binary Strings representing the file contents
 */
std::shared_ptr<Generator<std::string>> FileSystem::readFile(long bufferSize)
{

    // Create a return value
    std::shared_ptr<Generator<std::string>> retVal = nullptr;

    // Only continue if this is a file and exists
    if (exists() && isFile())
    {

        // Create a generator for getting the file contents
        auto fullPath = getFullPath();
        retVal = std::make_shared<Generator<std::string>>(
                [fullPath, bufferSize](std::shared_ptr<Yieldable<std::string>> yielder)
        {

            std::ifstream fileContents(fullPath);

            // Define the buffer size and read buffered-chunks
            char *buffer(new char[bufferSize]);
            while (fileContents && !fileContents.eof())
            {

                // Continuously read from the file until the desired
                // chunk/buffer size has been reached
                long actualSize = 0;
                std::string outBuffer;
                while ((actualSize < bufferSize) && !fileContents.eof())
                {

                    // Actually perform the buffered-chunk read
                    auto currSize = fileContents.readsome(buffer, (bufferSize - actualSize));
                    outBuffer += std::string(buffer, currSize);
                    actualSize += currSize;
                    fileContents.peek();
                }

                // Yield the current buffer results
                yielder->yield(outBuffer);
            }

            // Delete the temporary buffer
            delete[] buffer;

            // Complete the yielder
            yielder->complete();
        });
    }

    // Return the return value
    return retVal;
}

/**
 * Function used to write (stream) content to the disk as the current file
 * NOTE: Will fail if the file (this instance) already exists
 *
 * @param fileContent Generator of binary Strings representing the file contents
 * @return Boolean indicating whether the operation was successful or not
 */
bool FileSystem::writeFile(const std::shared_ptr<Generator<std::string>>& fileContent)
{

    // Create a return flag
    bool retFlag = false;

    // Only continue if this file does not exist already
    if (!exists())
    {

        // Create and open the file-reference
        FILE* pFile;
        pFile = std::fopen(getFullPath().c_str(), "wb");

        // Write out (stream) the file contents
        while (fileContent->hasMoreItems())
        {

            // Get the next chunk to write
            auto fileChunk = fileContent->getNextItem();

            // Actually write the next chunk
            fwrite(fileChunk.c_str(), 1, fileChunk.size(), pFile);
        }

        // Close the file-handle
        fclose(pFile);
        retFlag = true;
    }

    // Return the return flag
    return retFlag;
}
