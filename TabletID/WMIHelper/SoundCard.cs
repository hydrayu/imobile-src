using System;
using System.Collections;
using System.ComponentModel;
using System.Globalization;
using System.Management;
using System.Xml.Serialization;

namespace WMIHelper
{
    [TypeConverter(typeof (SoundCardConverter))]
    [ReadOnly(true)]
    public class SoundCard
    {
        private string productName;
        private string manufacturer;

        public SoundCard()
        {
        }

        public SoundCard(ManagementObject instance)
        {
            manufacturer = (instance.Properties["Manufacturer"].Value != null)
                               ? instance.Properties["Manufacturer"].Value.ToString().Trim()
                               : string.Empty;
            productName = (instance.Properties["ProductName"].Value != null)
                              ? instance.Properties["ProductName"].Value.ToString().Trim()
                              : string.Empty;
        }

        private SoundCard(string productName, string manufacturer)
        {
            this.productName = productName;
            this.manufacturer = manufacturer;
        }

        public override string ToString()
        {
            return manufacturer;
        }

        [Category("Sound Card Information")]
        [XmlElement()]
        public string ProductName
        {
            get { return productName; }
            set { productName = value; }
        }

        [Category("Sound Card Information")]
        [XmlElementAttribute()]
        public string Manufacturer
        {
            get { return manufacturer; }
            set { manufacturer = value; }
        }

        public class SoundCardConverter : ExpandableObjectConverter
        {
            public override object ConvertTo(ITypeDescriptorContext context, CultureInfo culture, object value,
                                             Type destinationType)
            {
                if (destinationType != typeof (string) || !(value is SoundCard))
                {
                    return base.ConvertTo(context, culture, value, destinationType);
                }
                return value.ToString().Trim();
            }

            public override object CreateInstance(ITypeDescriptorContext context, IDictionary propertyValues)
            {
                return
                    new SoundCard((string) propertyValues["SoundCard_ProductName"],
                                  (string) propertyValues["SoundCard_Manufacturer"]);
            }

            public override bool GetCreateInstanceSupported(ITypeDescriptorContext context)
            {
                return false;
            }
        }
    }
}
