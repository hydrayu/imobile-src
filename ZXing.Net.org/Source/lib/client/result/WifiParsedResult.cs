/*
 * Copyright 2010 ZXing authors
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

using System;
using System.Text;

namespace ZXing.Client.Result
{
    /// <summary>
    /// 
    /// </summary>
    /// <author>Vikram Aggarwal</author>
    public class WifiParsedResult : ParsedResult
    {
        /// <summary>
        /// WifiParsedResult() constructor
        /// </summary>
        /// <param name="networkEncryption"></param>
        /// <param name="ssid"></param>
        /// <param name="password"></param>
        public WifiParsedResult(String networkEncryption, String ssid, String password)
           : this(networkEncryption, ssid, password, false)
        {
        }
        /// <summary>
        /// WifiParsedResult() constructor
        /// </summary>
        /// <param name="networkEncryption"></param>
        /// <param name="ssid"></param>
        /// <param name="password"></param>
        /// <param name="hidden"></param>
        public WifiParsedResult(String networkEncryption, String ssid, String password, bool hidden)
           : base(ParsedResultType.WIFI)
        {
            Ssid = ssid;
            NetworkEncryption = networkEncryption;
            Password = password;
            Hidden = hidden;

            var result = new StringBuilder(80);
            maybeAppend(Ssid, result);
            maybeAppend(NetworkEncryption, result);
            maybeAppend(Password, result);
            maybeAppend(hidden.ToString(), result);
            displayResultValue = result.ToString();
        }
        /// <summary>
        /// Ssid()
        /// </summary>
        public String Ssid { get; private set; }
        /// <summary>
        /// NetworkEncryption()
        /// </summary>
        public String NetworkEncryption { get; private set; }
        /// <summary>
        /// Password()
        /// </summary>
        public String Password { get; private set; }
        /// <summary>
        /// Hidden()
        /// </summary>
        public bool Hidden { get; private set; }
    }
}