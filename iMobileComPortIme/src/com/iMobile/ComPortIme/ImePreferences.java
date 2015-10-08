/*
 * Copyright (C) 2011 The Android Open Source Project
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

package com.iMobile.ComPortIme;

import android.os.Bundle;
import android.preference.ListPreference;
import android.preference.Preference;
import android.preference.PreferenceActivity;
import android.preference.Preference.OnPreferenceChangeListener;
import android_serialport_api.SerialPortFinder;

/**
 * Displays the IME preferences inside the input method setting.
 */
public class ImePreferences extends PreferenceActivity {
	public SerialPortFinder mSerialPortFinder;
	private ListPreference m_comPorts;
	private ListPreference m_baudrates;
	private ListPreference m_inputdevice;
	
    @SuppressWarnings("deprecation")
	@Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // setTitle(R.string.settings_name);

        /*
         * General Preferences of COM PORT
         */        
        
		/* 
		 * 1. COM Ports
		 */

        mSerialPortFinder = new SerialPortFinder();
		addPreferencesFromResource(R.xml.ime_preferences);

		m_comPorts = (ListPreference)findPreference("DEVICE");
        String[] entries = mSerialPortFinder.getAllDevices();
        String[] entryValues = mSerialPortFinder.getAllDevicesPath();
        m_comPorts.setEntries(entries);
        m_comPorts.setEntryValues(entryValues);
		if (m_comPorts == null) {
	        for (int i=0; i<entryValues.length; ++i) {
	        	if (entryValues[i].indexOf("ttyMT0") >= 0) {
	        		m_comPorts.setSummary(entryValues[i]);
	        		m_comPorts.setValue(entryValues[i]);
	        		break;
	        	}
	        }
		}
		else m_comPorts.setSummary(m_comPorts.getValue());
        m_comPorts.setOnPreferenceChangeListener(new OnPreferenceChangeListener() {
			public boolean onPreferenceChange(Preference preference, Object newValue) {
				preference.setSummary((String)newValue);
				return true;
			}
		});

		/*
		 * 2. Baud rates
		 */
        m_baudrates = (ListPreference)findPreference("BAUDRATE");
        CharSequence[] m_baudrateEntries = m_baudrates.getEntries();
        if (m_baudrates == null) {
	        for (int i=0; i<m_baudrateEntries.length; ++i) {
	        	if (m_baudrateEntries[i].toString().indexOf("9600") >= 0) {
	        		m_baudrates.setSummary(m_baudrateEntries[i]);
	        		m_baudrates.setValue(m_baudrateEntries[i].toString());
	        		break;
	        	}
	        }
        }
		else m_baudrates.setSummary(m_baudrates.getValue());
        m_baudrates.setOnPreferenceChangeListener(new OnPreferenceChangeListener() {
			public boolean onPreferenceChange(Preference preference, Object newValue) {
				preference.setSummary((String)newValue);
				return true;
			}
		});
	        
        /*
         * SELECT A INPUT DEVICE
         */
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
				}
				return true;
			}
		});
    }
}
