/* This file is part of BitBoson.
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

#ifndef BITBOSON_STANDARDMODEL_BIGFLOAT_HPP
#define BITBOSON_STANDARDMODEL_BIGFLOAT_HPP

#include <boost/multiprecision/cpp_dec_float.hpp>

namespace BitBoson::StandardModel
{

    // Simply map the Boost Big-Float to our BigFloat
    typedef boost::multiprecision::cpp_dec_float_50 BigFloat;
}

#endif //BITBOSON_STANDARDMODEL_BIGFLOAT_HPP
