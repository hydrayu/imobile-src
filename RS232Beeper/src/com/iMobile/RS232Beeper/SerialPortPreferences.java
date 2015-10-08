/*
 * Copyright 2009 Cedric Priscal
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */

package com.iMobile.RS232Beeper;

import android.os.Bundle;
import android.preference.ListPreference;
import android.preference.Preference;
import android.preference.Preference.OnPreferenceChangeListener;
import android.preference.PreferenceActivity;
import android.util.Log;
import android_serialport_api.SerialPortFinder;
import android_serialport_api.SerialPort;

public class SerialPortPreferences extends PreferenceActivity {
    private String TAG="serial_port";
	private Application mApplication;
	private SerialPortFinder mSerialPortFinder;
	private ListPreference m_comPorts;
    private ListPreference m_baudrates;
    private ListPreference m_inputdevice;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Log.d(TAG, "SerialPortPreference/onCreate()");
		mApplication = (Application) getApplication();
		mSerialPortFinder = mApplication.mSerialPortFinder;

		addPreferencesFromResource(R.xml.serial_port_preferences);

		// Devices
		m_comPorts = (ListPreference)findPreference("PORT");
        String[] entries = mSerialPortFinder.getAllDevices();
        String[] entryValues = mSerialPortFinder.getAllDevicesPath();
        m_comPorts.setEntries(entries);
        m_comPorts.setEntryValues(entryValues);
        m_comPorts.setSummary(m_comPorts.getValue());
        m_comPorts.setOnPreferenceChangeListener(new OnPreferenceChangeListener() {
			public boolean onPreferenceChange(Preference preference, Object newValue) {
				preference.setSummary((String)newValue);
				return true;
			}
		});

		// Baud rates
        m_baudrates = (ListPreference)findPreference("BAUDRATE");
        m_baudrates.setSummary(m_baudrates.getValue());
        m_baudrates.setOnPreferenceChangeListener(new OnPreferenceChangeListener() {
			public boolean onPreferenceChange(Preference preference, Object newValue) {
				preference.setSummary((String)newValue);
				return true;
			}
		});
		
		// Input Devices
        m_inputdevice = (ListPreference)findPreference("INPUT_DEVICE");
        m_inputdevice.setOnPreferenceChangeListener(new OnPreferenceChangeListener() {
	        public boolean onPreferenceChange(Preference preference, Object newValue) {
                String selectDevice = (String)newValue;
                String[] devInfo = selectDevice.split(",");
                if (devInfo.length == 2) {
	                m_comPorts.setSummary(devInfo[0]);
	                m_comPorts.setValue(devInfo[0]);
	                m_baudrates.setSummary(devInfo[1]);
	                m_baudrates.setValue(devInfo[1]);
	                Log.d(TAG, "Set COM Port " + devInfo[0] + " / " + devInfo[1]);
                }
                return true;
	        }
        });
        if (m_comPorts.getSummary() == null || m_comPorts.getValue() == "") {
        	m_comPorts.setSummary("/dev/ttyUSB1");
        	m_comPorts.setValue("/dev/ttyUSB1");
        }
        if (m_baudrates.getSummary() == null || m_baudrates.getValue() == "") {
        	m_baudrates.setSummary("38400");
        	m_baudrates.setValue("38400");
        }
	}
}
