/*
 * Copyright 2014 ZXing authors
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
    /// class VINParsedResult
    /// </summary>
    public class VINParsedResult : ParsedResult
    {
        /// <summary>
        /// VIN
        /// </summary>
        public String VIN { get; private set; }
        /// <summary>
        /// WorldManufacturerID
        /// </summary>
        public String WorldManufacturerID { get; private set; }
        /// <summary>
        /// VehicleDescriptorSection
        /// </summary>
        public String VehicleDescriptorSection { get; private set; }
        /// <summary>
        /// VehicleIdentifierSection
        /// </summary>
        public String VehicleIdentifierSection { get; private set; }
        /// <summary>
        /// CountryCode
        /// </summary>
        public String CountryCode { get; private set; }
        /// <summary>
        /// VehicleAttributes
        /// </summary>
        public String VehicleAttributes { get; private set; }
        /// <summary>
        /// ModelYear
        /// </summary>
        public int ModelYear { get; private set; }
        /// <summary>
        /// PlantCode
        /// </summary>
        public char PlantCode { get; private set; }
        /// <summary>
        /// SequentialNumber
        /// </summary>
        public String SequentialNumber { get; private set; }
        /// <summary>
        /// VINParsedResult()
        /// </summary>
        /// <param name="vin"></param>
        /// <param name="worldManufacturerID"></param>
        /// <param name="vehicleDescriptorSection"></param>
        /// <param name="vehicleIdentifierSection"></param>
        /// <param name="countryCode"></param>
        /// <param name="vehicleAttributes"></param>
        /// <param name="modelYear"></param>
        /// <param name="plantCode"></param>
        /// <param name="sequentialNumber"></param>
        public VINParsedResult(String vin,
                               String worldManufacturerID,
                               String vehicleDescriptorSection,
                               String vehicleIdentifierSection,
                               String countryCode,
                               String vehicleAttributes,
                               int modelYear,
                               char plantCode,
                               String sequentialNumber)
           : base(ParsedResultType.VIN)
        {
            VIN = vin;
            WorldManufacturerID = worldManufacturerID;
            VehicleDescriptorSection = vehicleDescriptorSection;
            VehicleIdentifierSection = vehicleIdentifierSection;
            CountryCode = countryCode;
            VehicleAttributes = vehicleAttributes;
            ModelYear = modelYear;
            PlantCode = plantCode;
            SequentialNumber = sequentialNumber;
        }
        /// <summary>
        /// DisplayResult
        /// </summary>
        public override string DisplayResult
        {
            get
            {
                var result = new StringBuilder(50);
                result.Append(WorldManufacturerID).Append(' ');
                result.Append(VehicleDescriptorSection).Append(' ');
                result.Append(VehicleIdentifierSection).Append('\n');
                if (CountryCode != null)
                {
                    result.Append(CountryCode).Append(' ');
                }
                result.Append(ModelYear).Append(' ');
                result.Append(PlantCode).Append(' ');
                result.Append(SequentialNumber).Append('\n');
                return result.ToString();
            }
        }
    }
}