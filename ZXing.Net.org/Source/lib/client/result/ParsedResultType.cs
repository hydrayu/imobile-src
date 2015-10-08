/*
* Copyright 2007 ZXing authors
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

namespace ZXing.Client.Result
{
    /// <summary>
    /// Represents the type of data encoded by a barcode -- from plain text, to a
    /// URI, to an e-mail address, etc.
    /// </summary>
    /// <author>Sean Owen</author>
    public enum ParsedResultType
    {
        /// <summary>
        /// ADDRESSBOOK = 0
        /// </summary>
        ADDRESSBOOK,
        /// <summary>
        /// EMAIL_ADDRESS = 1
        /// </summary>
        EMAIL_ADDRESS,
        /// <summary>
        /// PRODUCT = 2
        /// </summary>
        PRODUCT,
        /// <summary>
        /// URI = 3
        /// </summary>
        URI,
        /// <summary>
        /// TEXT = 4
        /// </summary>
        TEXT,
        /// <summary>
        /// GEO = 5
        /// </summary>
        GEO,
        /// <summary>
        /// TEL = 6
        /// </summary>
        TEL,
        /// <summary>
        /// SMS=7
        /// </summary>
        SMS,
        /// <summary>
        /// CALENDAR=8
        /// </summary>
        CALENDAR,
        /// <summary>
        /// WIFI=9
        /// </summary>
        WIFI,
        /// <summary>
        /// ISBN=10
        /// </summary>
        ISBN,
        /// <summary>
        /// VIN=11
        /// </summary>
        VIN
    }
}