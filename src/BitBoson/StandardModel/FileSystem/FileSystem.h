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

#ifndef BITBOSON_STANDARDMODEL_FILESYSTEM_H
#define BITBOSON_STANDARDMODEL_FILESYSTEM_H

#include <string>
#include <BitBoson/StandardModel/Primitives/Generator.hpp>

namespace BitBoson::StandardModel
{

    class FileSystem
    {

        // Private member variables
        private:
            std::string _fullPath;

        // Public member functions
        public:

            /**
             * Constructor used to setup the FileSystem object on the provided directory
             *
             * @param directory String representing the file-system object
             */
            explicit FileSystem(const std::string& directory);

            /**
             * Static function used to get a temporary directory FileSystem instance
             * NOTE: This is a managed directory which will be deleted on its own
             *       You can (and it is recommended you do) delete it on your own though
             *
             * @param prefix String representing the desired prefix for the random directory
             * @return FileSystem representing the temporary directory
             */
            static FileSystem getTemporaryDir(const std::string& prefix);

            /**
             * Function used to get the full path string of the FilesSystem instance
             *
             * @return String representing the full path string of the FilesSystem instance
             */
            std::string getFullPath() const;

            /**
             * Function used to check whether the setup FileSystem instance is a directory
             *
             * @return Boolean indicating whether the setup FileSystem instance is a directory
             */
            bool isDirectory() const;

            /**
             * Function used to check whether the setup FileSystem instance is a directory
             *
             * @return Boolean indicating whether the setup FileSystem instance is a directory
             */
            bool isFile() const;

            /**
             * Function used to check whether the setup FileSystem instance exists
             * NOTE: This will be true if the FileSystem is either a directory or a file
             *
             * @return Boolean indicating whether the setup FileSystem instance exists
             */
            bool exists() const;

            /**
             * Function used to get the specified child directory/file of the FileSystem instance
             *
             * @param child String representing the child object to create the new instance on
             * @return FileSystem representing the child directory/file of the FileSystem instance
             */
            FileSystem getChild(const std::string& child) const;

            /**
             * Function used to remove the instance's FileSystem directory
             * NOTE: This will not remove the instance if it is a file not a directory
             *
             * @return Boolean indicating whether the operation was successful or not
             */
            bool removeDir();

            /**
             * Function used to remove the instance's FileSystem file
             * NOTE: This will not remove the instance if it is a directory not a file
             *
             * @return Boolean indicating whether the operation was successful or not
             */
            bool removeFile();

            /**
             * Function used to create instance's FileSystem as a directory
             *
             * @return Boolean indicating whether the operation was successful or not
             */
            bool createDir();

            /**
             * Function used to rename the current instance's reference (file or directory)
             *
             * @param newName String representing the instance's new reference name
             * @return Boolean indicating whether the operation was successful or not
             */
            bool rename(const std::string& newName);

            /**
             * Function used to read-in (stream) the current object as a file
             * NOTE: This will return an empty generator if the item is not a file
             *
             * @param bufferSize Long representing the buffer size to use when reading
             * @return Generator of binary Strings representing the file contents
             */
            std::shared_ptr<Generator<std::string>> readFile(long bufferSize=(1024*1024));

            /**
             * Function used to write (stream) content to the disk as the current file
             * NOTE: Will fail if the file (this instance) already exists
             *
             * @param fileContent Generator of binary Strings representing the file contents
             * @return Boolean indicating whether the operation was successful or not
             */
            bool writeFile(const std::shared_ptr<Generator<std::string>>& fileContent);

        // Private member functions
        private:

            /**
             * Hidden Constructor ued to setup an empty instance of the FileSystem object
             */
            FileSystem() = default;

    };
}

#endif //BITBOSON_STANDARDMODEL_FILESYSTEM_H
