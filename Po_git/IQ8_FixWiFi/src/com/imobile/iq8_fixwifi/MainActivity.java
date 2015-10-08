package com.imobile.iq8_fixwifi;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.util.Random;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.res.AssetManager;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {
	private final String TAG = "Po_debug";
	private TextView Po_Mac_TV1;
	private int Po_Mac_status = 0;
	private String org_mac = "";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		Po_find_id();
		open_wifi(this);
		check_device_mac();
		// 1. copy file to /sdcard
		// CopyAssets();
	}

	private void open_wifi(Context context) {
		WifiManager wifiManager = (WifiManager) context
				.getSystemService(Context.WIFI_SERVICE);
		if (!wifiManager.isWifiEnabled()) {
			wifiManager.setWifiEnabled(true);

		}
	}

	private void check_device_mac() {

		// 1. copy ini to /mnt/shell/emulated/0/
		Run_su("busybox cp /data/misc/wifi/WCNSS_qcom_cfg.ini /mnt/shell/emulated/0/org.ini");

		// 2. Get the text file
		// P.S /mnt/shell/emulated/0/org.ini == /data/media/0/org.ini
		File file = new File("/data/media/0/org.ini");

		// Read text from file
		StringBuilder text = new StringBuilder();

		try {

			BufferedReader br = new BufferedReader(new FileReader(file));
			String line;

			while ((line = br.readLine()) != null) {
				if (line.indexOf("Intf0MacAddress=") != -1) {
					// 3. append Mac address
					text.append(line);
					// text.append('\n');
				}

			}
			br.close();
		} catch (IOException e) {
			Log.i(TAG, "e=" + e);
			// You'll need to add proper error handling here
		}

		// 4. spplit Mac address
		// String[] tokens = new String[] {text.toString()};
		String[] tokens = text.toString().split("Intf0MacAddress=");

		for (String token : tokens) {
			org_mac += token;
			// System.out.println(token);
		}

		Log.i(TAG, "org_mac=" + org_mac);

		// 5. show Mac Address to layout
		if ((org_mac != null) || (org_mac != "")) {
			String show_mac = "";

			show_mac += org_mac.substring(0, 2) + ":";
			show_mac += org_mac.substring(2, 4) + ":";
			show_mac += org_mac.substring(4, 6) + ":";
			show_mac += org_mac.substring(6, 8) + ":";
			show_mac += org_mac.substring(8, 10) + ":";
			show_mac += org_mac.substring(10, 12);

			this.Po_Mac_TV1.setText(show_mac);
		} else {
			this.Po_Mac_TV1.setText("null");
		}

		// 6. check Mac address
		// 0 --> error (need flash Mac)
		// 1 --> success (can`t flash)
		if (org_mac.substring(0, 6).equals("000AF5")) {
			this.Po_Mac_status = 0;
		} else {
			this.Po_Mac_status = 1;
		}

		// 7. rm /mnt/shell/emulated/0/org.ini
		Run_su("busybox rm /mnt/shell/emulated/0/org.ini");
	}

	private String Po_get_Mac() {

		if (this.Po_Mac_status == 0) {
			// need flash new Mac address
			char[] chars = "ABCDEF0123456789".toCharArray();
			StringBuilder sb = new StringBuilder();
			Random random = new Random();
			for (int i = 0; i < 6; i++) {
				char c = chars[random.nextInt(chars.length)];
				sb.append(c);
			}
			String output = sb.toString();

			String My_New_Mac = "6002B4" + output;
			Log.i(TAG, "My_Mac=" + My_New_Mac);
			return My_New_Mac;

		}

		Log.i(TAG, "(org)My_Mac=" + org_mac);
		return org_mac;
	}

	public void Po_find_id() {
		this.Po_Mac_TV1 = (TextView) findViewById(R.id.Po_Mac_TV1);
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		Log.i(TAG, "onDestroy()");

		// delete /mnt/shell/emulated/0/*.ini & disable_WMM & enable_WMM && call
		// reboot()
		Run_su("busybox rm /mnt/shell/emulated/0/*_WMM; busybox rm /mnt/shell/emulated/0/WCNSS_qcom_cfg_Jason1.ini; busybox rm /mnt/shell/emulated/0/WCNSS_qcom_cfg_org.ini; /system/bin/reboot");
		// Kill myself
		// android.os.Process.killProcess(android.os.Process.myPid());
	}

	/***
	 * 顯示diglog
	 * 
	 * @param Title
	 * @param Msg
	 */
	public void show_dialog(String Title, String Msg) {
		AlertDialog.Builder dialog = new AlertDialog.Builder(this);
		dialog.setTitle(Title);
		dialog.setMessage(Msg);
		dialog.setIcon(android.R.drawable.ic_dialog_alert);
		dialog.setCancelable(false);
		dialog.setPositiveButton("OK", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int which) {
				// 按下PositiveButton要做的事
				Toast.makeText(MainActivity.this, "APP is exit",
						Toast.LENGTH_SHORT).show();
				onDestroy();
			}
		});

		dialog.show();

	}

	/**
	 * check_copy_dialog
	 * 
	 * @param Title
	 * @param Msg
	 * @param config_num
	 */
	public void check_copy_dialog(String Title, String Msg, int config_num) {
		AlertDialog.Builder dialog = new AlertDialog.Builder(this);
		dialog.setTitle(Title);
		dialog.setMessage(Msg);
		dialog.setIcon(android.R.drawable.ic_dialog_alert);
		dialog.setCancelable(false);
		if (config_num == 1) {
			// user select config1
			Log.i(TAG, "config_num = 1");

			dialog.setPositiveButton("OK",
					new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog, int which) {
							// 按下PositiveButton要做的事
							// 1. creat a cfg (disable_WMM)

							Run_su("echo '# This file allows user to override the factory\n\n# defaults for the WLAN Driver\n\n\n# Enable IMPS or not\ngEnableImps=1\n\n# Enable/Disable Idle Scan\n\ngEnableIdleScan=0\n\n\n# Increase sleep duration (seconds) during IMPS\n# 0 implies no periodic wake up from IMPS. Periodic wakeup is \n# unnecessary if Idle Scan is disabled.\ngImpsModSleepTime=0\n\n\n# Enable BMPS or not\ngEnableBmps=1\n\n# Enable suspend or not\n\n# 1: Enable standby, 2: Enable Deep sleep, 3: Enable Mcast/Bcast Filter\n\ngEnableSuspend=3\n\n\n# Phy Mode (auto, b, g, n, etc)\n# Valid values are 0-9, with 0 = Auto, 4 = 11n, 9 = 11ac\ngDot11Mode=0\n\n\n# Handoff Enable(1) Disable(0)\n\ngEnableHandoff=0\n\n\n# CSR Roaming Enable(1) Disable(0)\n\ngRoamingTime=0\n\n\n# Assigned MAC Addresses - This will be used until NV items are in place\n\n# Each byte of MAC address is represented in Hex format as XX\n\nIntf0MacAddress="
									+ Po_get_Mac()
									+ "\nIntf1MacAddress=000AF58989FE\nIntf2MacAddress=000AF58989FD\n\nIntf3MacAddress=000AF58989FC\n\n\n# UAPSD service interval for VO,VI, BE, BK traffic\n\nInfraUapsdVoSrvIntv=0\n\nInfraUapsdViSrvIntv=0\n\nInfraUapsdBeSrvIntv=0\n\nInfraUapsdBkSrvIntv=0\n\n\n# Make 1x1 the default antenna configuration\n\ngNumRxAnt=1\n\n\n# Beacon filtering frequency (unit in beacon intervals)\n\ngNthBeaconFilter=50\n\n\n# Enable WAPI or not\n\n# WAPIIsEnabled=0\n\n\n# Flags to filter Mcast abd Bcast RX packets.\n\n# Value 0: No filtering, 1: Filter all Multicast.\n\n# 2: Filter all Broadcast. 3: Filter all Mcast abd Bcast\n\nMcastBcastFilter=3\n\n\n#Flag to enable HostARPOffload feature or not\n\nhostArpOffload=1\n\n\n#SoftAP Related Parameters\n\n# AP MAc addr\n\ngAPMacAddr=000AF589dcab\n\n\n# 802.11n Protection flag\n\ngEnableApProt=1\n\n\n#Enable OBSS protection\n\ngEnableApOBSSProt=1\n\n\n#Enable/Disable UAPSD for SoftAP\n\ngEnableApUapsd=0\n\n\n# Fixed Rate\n\ngFixedRate=0\n\n\n# Maximum Tx power\n\n# gTxPowerCap=30\n\n\n# Fragmentation Threshold\n\n# gFragmentationThreshold=2346\n\n\n# RTS threshold\n\nRTSThreshold=2347\n\n\n# Intra-BSS forward\n\ngDisableIntraBssFwd=0\n\n\n# WMM Enable/Disable\n\nWmmIsEnabled=0\n\n\n# 802.11d support\n\ng11dSupportEnabled=1\n\n# 802.11h support\n\ng11hSupportEnabled=1\n\n# CCX Support and fast transition\nCcxEnabled=0\nFastTransitionEnabled=1\nImplicitQosIsEnabled=1\ngNeighborScanTimerPeriod=200\n\ngNeighborLookupThreshold=76\ngNeighborReassocThreshold=81\n\ngNeighborScanChannelMinTime=20\ngNeighborScanChannelMaxTime=30\ngMaxNeighborReqTries=3\n\n# Legacy (non-CCX, non-802.11r) Fast Roaming Support\n# To enable, set FastRoamEnabled=1\n# To disable, set FastRoamEnabled=0\nFastRoamEnabled=1\n\n#Check if the AP to which we are roaming is better than current AP in terms of RSSI.\n#Checking is disabled if set to Zero.Otherwise it will use this value as to how better \n#the RSSI of the new/roamable AP should be for roaming\nRoamRssiDiff=3\n\n# If the RSSI of any available candidate is better than currently associated\n# AP by at least gImmediateRoamRssiDiff, then being to roam immediately (without\n# registering for reassoc threshold).\n# NOTE: Value of 0 means that we would register for reassoc threshold.\ngImmediateRoamRssiDiff=10\n\n# To enable, set gRoamIntraBand=1 (Roaming within band)\n# To disable, set gRoamIntraBand=0 (Roaming across band)\ngRoamIntraBand=0\n\n# SAP Country code\n\n# Default Country Code is 2 bytes, 3rd byte is optional indoor or out door.\n\n# Example\n\n#   US Indoor, USI\n\n#   Korea Outdoor, KRO\n\n#   Japan without optional byte, JP\n\n#   France without optional byte, FR\n\n#gAPCntryCode=USI\n\n\n#Short Guard Interval Enable/disable\n\ngShortGI20Mhz=1\n\ngShortGI40Mhz=1\n\n\n#Auto Shutdown  Value in seconds. A value of 0 means Auto shutoff is disabled\n\ngAPAutoShutOff=0\n\n\n# SAP auto channel selection configuration\n\n# 0 = disable auto channel selection\n\n# 1 = enable auto channel selection, channel provided by supplicant will be ignored\n\ngApAutoChannelSelection=0\n\n\n# Listen Energy Detect Mode Configuration\n\n# Valid values 0-128\n\n# 128 means disable Energy Detect feature\n\n# 0-9 are threshold code and 7 is recommended value from system if feature is to be enabled.\n\n# 10-128 are reserved.\n\n# The EDET threshold mapping is as follows in 3dB step:\n\n# 0 = -60 dBm\n\n# 1 = -63 dBm\n\n# 2 = -66 dBm\n\n# ...\n\n# 7 = -81 dBm\n\n# 8 = -84 dBm\n\n# 9 = -87 dBm\n\n# Note: Any of these settings are valid. Setting 0 would yield the highest power saving (in a noisy environment) at the cost of more range. The range impact is approximately #calculated as:\n\n#\n\n#  Range Loss  (dB)  =  EDET threshold level (dBm) + 97 dBm.\n\n#\n\ngEnablePhyAgcListenMode=128\n\n\n#Preferred channel to start BT AMP AP mode (0 means, any channel)\n\nBtAmpPreferredChannel=0\n\n\n#Preferred band (both or 2.4 only or 5 only)\n\nBandCapability=0\n\n\n#Beacon Early Termination (1 = enable the BET feature, 0 = disable)\n\nenableBeaconEarlyTermination=1\n\ngTelescopicBeaconWakeupEn=1\ntelescopicBeaconTransListenInterval=3\ntelescopicBeaconTransListenIntervalNumIdleBcns=10\ntelescopicBeaconMaxListenInterval=5\ntelescopicBeaconMaxListenIntervalNumIdleBcns=15\n\nbeaconEarlyTerminationWakeInterval=10\n\n\n#Bluetooth Alternate Mac Phy (1 = enable the BT AMP feature, 0 = disable)\n\ngEnableBtAmp=0\n\n\n#SOFTAP Channel Range selection \n\ngAPChannelSelectStartChannel=1\n\ngAPChannelSelectEndChannel=11\n\n\n#SOFTAP Channel Range selection Operating band\n\n# 0:2.4GHZ 1: LOW-5GHZ 2:MID-5GHZ 3:HIGH-5GHZ 4: 4.9HZ BAND\n\ngAPChannelSelectOperatingBand=0\n\n\n#Channel Bonding\ngChannelBondingMode5GHz=1\n\n\n#Enable Keep alive with non-zero period value\n\n#gStaKeepAlivePeriod = 30\n\n#AP LINK MONITOR TIMEOUT is used for both SAP and GO mode.\n#It is used to change the frequency of keep alive packets in the AP Link Monitor period which is by\n#default 20s. Currently the keep alive packets are sent as an interval of 3s but after this change\n#the keep alive packet frequency can be changed.\n\n#gApLinkMonitorPeriod = 3\n\n\n#If set will start with active scan after driver load, otherwise will start with\n\n#passive scan to find out the domain\n\ngEnableBypass11d=1\n\n\n#If set to 0, will not scan DFS channels\n\ngEnableDFSChnlScan=1\n\n\ngVhtChannelWidth=2\ngEnableLogp=1\n\n\n# Enable Automatic Tx Power control\n\ngEnableAutomaticTxPowerControl=1\n\n# 0 for OLPC 1 for CLPC and SCPC\ngEnableCloseLoop=1\n\n#Data Inactivity Timeout when in powersave (in ms)\ngDataInactivityTimeout=200\n\n# VHT Tx/Rx MCS values\n# Valid values are 0,1,2. If commented out, the default value is 0.\n# 0=MCS0-7, 1=MCS0-8, 2=MCS0-9\ngVhtRxMCS=2\ngVhtTxMCS=2\n\n# Enable CRDA regulatory support by settings default country code\n#gCrdaDefaultCountryCode=TW\n\n# Scan Timing Parameters\n# gPassiveMaxChannelTime=110\n# gPassiveMinChannelTime=60\n# gActiveMaxChannelTime=40\n# gActiveMinChannelTime=20\n\n#If set to 0, MCC is not allowed.\ngEnableMCCMode=1\n\n# 1=enable STBC; 0=disable STBC \ngEnableRXSTBC=1\n\n# Enable Active mode offload\ngEnableActiveModeOffload=1\n\n#Enable Scan Results Aging based on timer \n#Timer value is in seconds\n#If Set to 0 it will not enable the feature\ngScanAgingTime=0\n\n#Enable Power saving mechanism Based on Android Framework\n#If set to 0 Driver internally control the Power saving mechanism\n#If set to 1 Android Framwrok control the Power saving mechanism\nisAndroidPsEn=0\n\n#disable LDPC in STA mode if the AP is TXBF capable\ngDisableLDPCWithTxbfAP=1\n\n#Enable thermal mitigation\ngThermalMitigationEnable=1\n\n#List of Country codes for which 11ac needs to be disabled\n#Each country code must be delimited by comma(,)\ngListOfNon11acCountryCode=RU,UA,ZA\n\n#Maxium Channel time in msec\ngMaxMediumTime = 6000\n\n# 802.11K support\ngRrmEnable=1\ngRrmOperChanMax=8\ngRrmNonOperChanMax=8\ngRrmRandIntvl=100\n\nEND\n\n# Note: Configuration parser would not read anything past the END marker\n\n' > /mnt/shell/emulated/0/disable_WMM");

							// 2. chang ini to WCNSS_qcom_cfg_Jason1.ini
							Run_su("busybox rm /data/misc/wifi/WCNSS_qcom_cfg.ini; busybox mv /mnt/shell/emulated/0/disable_WMM /data/misc/wifi/WCNSS_qcom_cfg.ini; busybox chmod 777 /data/misc/wifi/WCNSS_qcom_cfg.ini");

							// 3. copy to /system/
							Run_su("busybox cp -f /data/misc/wifi/WCNSS_qcom_cfg.ini /system/WCNSS_qcom_cfg.ini; echo 'PATH=/sbin:/vendor/bin:/system/sbin:/system/bin:/system/xbin\n\nsleep 20\nbusybox cp -f /system/WCNSS_qcom_cfg.ini /data/misc/wifi/WCNSS_qcom_cfg.ini' > /system/local_script/chang_WCNSS_qcom_cfg.sh; chmod -R 777 /system/local_script/chang_WCNSS_qcom_cfg.sh");

							show_dialog("WiFi config1 updata success",
									"system will reboot!!");
						}
					});
		} else {
			// user select config2
			Log.i(TAG, "config_num = 2");
			dialog.setPositiveButton("OK",
					new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog, int which) {
							// 按下PositiveButton要做的事

							// 1. creat a cfg (enable_WMM)

							Run_su("echo '# This file allows user to override the factory\n\n# defaults for the WLAN Driver\n\n\n# Enable IMPS or not\ngEnableImps=1\n\n# Enable/Disable Idle Scan\n\ngEnableIdleScan=0\n\n\n# Increase sleep duration (seconds) during IMPS\n# 0 implies no periodic wake up from IMPS. Periodic wakeup is \n# unnecessary if Idle Scan is disabled.\ngImpsModSleepTime=0\n\n\n# Enable BMPS or not\ngEnableBmps=1\n\n# Enable suspend or not\n\n# 1: Enable standby, 2: Enable Deep sleep, 3: Enable Mcast/Bcast Filter\n\ngEnableSuspend=3\n\n\n# Phy Mode (auto, b, g, n, etc)\n# Valid values are 0-9, with 0 = Auto, 4 = 11n, 9 = 11ac\ngDot11Mode=0\n\n\n# Handoff Enable(1) Disable(0)\n\ngEnableHandoff=0\n\n\n# CSR Roaming Enable(1) Disable(0)\n\ngRoamingTime=0\n\n\n# Assigned MAC Addresses - This will be used until NV items are in place\n\n# Each byte of MAC address is represented in Hex format as XX\n\nIntf0MacAddress="
									+ Po_get_Mac()
									+ "\nIntf1MacAddress=000AF58989FE\nIntf2MacAddress=000AF58989FD\n\nIntf3MacAddress=000AF58989FC\n\n\n# UAPSD service interval for VO,VI, BE, BK traffic\n\nInfraUapsdVoSrvIntv=0\n\nInfraUapsdViSrvIntv=0\n\nInfraUapsdBeSrvIntv=0\n\nInfraUapsdBkSrvIntv=0\n\n\n# Make 1x1 the default antenna configuration\n\ngNumRxAnt=1\n\n\n# Beacon filtering frequency (unit in beacon intervals)\n\ngNthBeaconFilter=50\n\n\n# Enable WAPI or not\n\n# WAPIIsEnabled=0\n\n\n# Flags to filter Mcast abd Bcast RX packets.\n\n# Value 0: No filtering, 1: Filter all Multicast.\n\n# 2: Filter all Broadcast. 3: Filter all Mcast abd Bcast\n\nMcastBcastFilter=3\n\n\n#Flag to enable HostARPOffload feature or not\n\nhostArpOffload=1\n\n\n#SoftAP Related Parameters\n\n# AP MAc addr\n\ngAPMacAddr=000AF589dcab\n\n\n# 802.11n Protection flag\n\ngEnableApProt=1\n\n\n#Enable OBSS protection\n\ngEnableApOBSSProt=1\n\n\n#Enable/Disable UAPSD for SoftAP\n\ngEnableApUapsd=0\n\n\n# Fixed Rate\n\ngFixedRate=0\n\n\n# Maximum Tx power\n\n# gTxPowerCap=30\n\n\n# Fragmentation Threshold\n\n# gFragmentationThreshold=2346\n\n\n# RTS threshold\n\nRTSThreshold=2347\n\n\n# Intra-BSS forward\n\ngDisableIntraBssFwd=0\n\n\n# WMM Enable/Disable\n\nWmmIsEnabled=1\n\n\n# 802.11d support\n\ng11dSupportEnabled=1\n\n# 802.11h support\n\ng11hSupportEnabled=1\n\n# CCX Support and fast transition\nCcxEnabled=0\nFastTransitionEnabled=1\nImplicitQosIsEnabled=1\ngNeighborScanTimerPeriod=200\n\ngNeighborLookupThreshold=76\ngNeighborReassocThreshold=81\n\ngNeighborScanChannelMinTime=20\ngNeighborScanChannelMaxTime=30\ngMaxNeighborReqTries=3\n\n# Legacy (non-CCX, non-802.11r) Fast Roaming Support\n# To enable, set FastRoamEnabled=1\n# To disable, set FastRoamEnabled=0\nFastRoamEnabled=1\n\n#Check if the AP to which we are roaming is better than current AP in terms of RSSI.\n#Checking is disabled if set to Zero.Otherwise it will use this value as to how better \n#the RSSI of the new/roamable AP should be for roaming\nRoamRssiDiff=3\n\n# If the RSSI of any available candidate is better than currently associated\n# AP by at least gImmediateRoamRssiDiff, then being to roam immediately (without\n# registering for reassoc threshold).\n# NOTE: Value of 0 means that we would register for reassoc threshold.\ngImmediateRoamRssiDiff=10\n\n# To enable, set gRoamIntraBand=1 (Roaming within band)\n# To disable, set gRoamIntraBand=0 (Roaming across band)\ngRoamIntraBand=0\n\n# SAP Country code\n\n# Default Country Code is 2 bytes, 3rd byte is optional indoor or out door.\n\n# Example\n\n#   US Indoor, USI\n\n#   Korea Outdoor, KRO\n\n#   Japan without optional byte, JP\n\n#   France without optional byte, FR\n\n#gAPCntryCode=USI\n\n\n#Short Guard Interval Enable/disable\n\ngShortGI20Mhz=1\n\ngShortGI40Mhz=1\n\n\n#Auto Shutdown  Value in seconds. A value of 0 means Auto shutoff is disabled\n\ngAPAutoShutOff=0\n\n\n# SAP auto channel selection configuration\n\n# 0 = disable auto channel selection\n\n# 1 = enable auto channel selection, channel provided by supplicant will be ignored\n\ngApAutoChannelSelection=0\n\n\n# Listen Energy Detect Mode Configuration\n\n# Valid values 0-128\n\n# 128 means disable Energy Detect feature\n\n# 0-9 are threshold code and 7 is recommended value from system if feature is to be enabled.\n\n# 10-128 are reserved.\n\n# The EDET threshold mapping is as follows in 3dB step:\n\n# 0 = -60 dBm\n\n# 1 = -63 dBm\n\n# 2 = -66 dBm\n\n# ...\n\n# 7 = -81 dBm\n\n# 8 = -84 dBm\n\n# 9 = -87 dBm\n\n# Note: Any of these settings are valid. Setting 0 would yield the highest power saving (in a noisy environment) at the cost of more range. The range impact is approximately #calculated as:\n\n#\n\n#  Range Loss  (dB)  =  EDET threshold level (dBm) + 97 dBm.\n\n#\n\ngEnablePhyAgcListenMode=128\n\n\n#Preferred channel to start BT AMP AP mode (0 means, any channel)\n\nBtAmpPreferredChannel=0\n\n\n#Preferred band (both or 2.4 only or 5 only)\n\nBandCapability=0\n\n\n#Beacon Early Termination (1 = enable the BET feature, 0 = disable)\n\nenableBeaconEarlyTermination=1\n\ngTelescopicBeaconWakeupEn=1\ntelescopicBeaconTransListenInterval=3\ntelescopicBeaconTransListenIntervalNumIdleBcns=10\ntelescopicBeaconMaxListenInterval=5\ntelescopicBeaconMaxListenIntervalNumIdleBcns=15\n\nbeaconEarlyTerminationWakeInterval=10\n\n\n#Bluetooth Alternate Mac Phy (1 = enable the BT AMP feature, 0 = disable)\n\ngEnableBtAmp=0\n\n\n#SOFTAP Channel Range selection \n\ngAPChannelSelectStartChannel=1\n\ngAPChannelSelectEndChannel=11\n\n\n#SOFTAP Channel Range selection Operating band\n\n# 0:2.4GHZ 1: LOW-5GHZ 2:MID-5GHZ 3:HIGH-5GHZ 4: 4.9HZ BAND\n\ngAPChannelSelectOperatingBand=0\n\n\n#Channel Bonding\ngChannelBondingMode5GHz=1\n\n\n#Enable Keep alive with non-zero period value\n\n#gStaKeepAlivePeriod = 30\n\n#AP LINK MONITOR TIMEOUT is used for both SAP and GO mode.\n#It is used to change the frequency of keep alive packets in the AP Link Monitor period which is by\n#default 20s. Currently the keep alive packets are sent as an interval of 3s but after this change\n#the keep alive packet frequency can be changed.\n\n#gApLinkMonitorPeriod = 3\n\n\n#If set will start with active scan after driver load, otherwise will start with\n\n#passive scan to find out the domain\n\ngEnableBypass11d=1\n\n\n#If set to 0, will not scan DFS channels\n\ngEnableDFSChnlScan=1\n\n\ngVhtChannelWidth=2\ngEnableLogp=1\n\n\n# Enable Automatic Tx Power control\n\ngEnableAutomaticTxPowerControl=1\n\n# 0 for OLPC 1 for CLPC and SCPC\ngEnableCloseLoop=1\n\n#Data Inactivity Timeout when in powersave (in ms)\ngDataInactivityTimeout=200\n\n# VHT Tx/Rx MCS values\n# Valid values are 0,1,2. If commented out, the default value is 0.\n# 0=MCS0-7, 1=MCS0-8, 2=MCS0-9\ngVhtRxMCS=2\ngVhtTxMCS=2\n\n# Enable CRDA regulatory support by settings default country code\n#gCrdaDefaultCountryCode=TW\n\n# Scan Timing Parameters\n# gPassiveMaxChannelTime=110\n# gPassiveMinChannelTime=60\n# gActiveMaxChannelTime=40\n# gActiveMinChannelTime=20\n\n#If set to 0, MCC is not allowed.\ngEnableMCCMode=1\n\n# 1=enable STBC; 0=disable STBC \ngEnableRXSTBC=1\n\n# Enable Active mode offload\ngEnableActiveModeOffload=1\n\n#Enable Scan Results Aging based on timer \n#Timer value is in seconds\n#If Set to 0 it will not enable the feature\ngScanAgingTime=0\n\n#Enable Power saving mechanism Based on Android Framework\n#If set to 0 Driver internally control the Power saving mechanism\n#If set to 1 Android Framwrok control the Power saving mechanism\nisAndroidPsEn=0\n\n#disable LDPC in STA mode if the AP is TXBF capable\ngDisableLDPCWithTxbfAP=1\n\n#Enable thermal mitigation\ngThermalMitigationEnable=1\n\n#List of Country codes for which 11ac needs to be disabled\n#Each country code must be delimited by comma(,)\ngListOfNon11acCountryCode=RU,UA,ZA\n\n#Maxium Channel time in msec\ngMaxMediumTime = 6000\n\n# 802.11K support\ngRrmEnable=1\ngRrmOperChanMax=8\ngRrmNonOperChanMax=8\ngRrmRandIntvl=100\n\nEND\n\n# Note: Configuration parser would not read anything past the END marker\n\n' > /mnt/shell/emulated/0/enable_WMM");

							// 2. chang ini
							Run_su("busybox rm /data/misc/wifi/WCNSS_qcom_cfg.ini; busybox mv /mnt/shell/emulated/0/enable_WMM /data/misc/wifi/WCNSS_qcom_cfg.ini; busybox chmod 777 /data/misc/wifi/WCNSS_qcom_cfg.ini");

							// 3. copy to /system/
							Run_su("busybox cp -f /data/misc/wifi/WCNSS_qcom_cfg.ini /system/WCNSS_qcom_cfg.ini; echo 'PATH=/sbin:/vendor/bin:/system/sbin:/system/bin:/system/xbin\n\nsleep 20\nbusybox cp -f /system/WCNSS_qcom_cfg.ini /data/misc/wifi/WCNSS_qcom_cfg.ini' > /system/local_script/chang_WCNSS_qcom_cfg.sh; chmod -R 777 /system/local_script/chang_WCNSS_qcom_cfg.sh");
							show_dialog("WiFi config2 updata success",
									"system will reboot!!");
						}
					});
		}

		dialog.show();

	}

	public void Toast(String str) {
		Toast.makeText(this, str, Toast.LENGTH_SHORT).show();
	}

	public void Run_su(String str) {
		Runtime ex = Runtime.getRuntime();
		String cmdBecomeSu = "su";
		String script = str;
		Log.e("Debug", "Run_su()");
		try {
			java.lang.Process runsum = ex.exec(cmdBecomeSu);
			int exitVal = 0;
			final OutputStreamWriter out = new OutputStreamWriter(
					runsum.getOutputStream());
			// Write the script to be executed
			out.write(script);
			// Ensure that the last character is an "enter"
			out.write("\n");
			out.flush();
			// Terminate the "su" process
			out.write("exit\n");
			out.flush();
			exitVal = runsum.waitFor();
			if (exitVal == 0) {
				Log.e("Debug", "Successfully to su");
				Toast("Successfully to su");
			}
		} catch (Exception e) {
			Log.e("Debug", "Fails to su");
			Toast("Fails to su");
		}
	}

	/***
	 * Po_Config1
	 * 
	 * @param view
	 *            button1
	 */
	public void Po_Config1(View view) {
		Log.i(TAG, "Po_Config1()");

		// check Mac number = 12
		if (Po_get_Mac().length() != 12) {
			Toast("Please input Mac Adderss!!" + Po_get_Mac().length());
		} else {
			check_copy_dialog(
					"Are you sure you want to replace wifi config??",
					"Will to replace wifi config to Config1!\nif config updata is succuss!! want to reboot system!",
					1);
		}

	}

	/***
	 * Po_Config2
	 * 
	 * @param view
	 *            button2
	 */
	public void Po_Config2(View view) {
		Log.i(TAG, "Po_Config2()");
		check_copy_dialog(
				"Are you sure you want to replace wifi config??",
				"Will to replace wifi config to Config2!\nif config updata is succuss!! want to reboot system!",
				2);
	}

	private void CopyAssets() {
		AssetManager assetManager = getAssets();
		String[] files = null;
		try {
			files = assetManager.list("Files");
		} catch (IOException e) {
			Log.e(TAG, e.getMessage());
		}

		for (String filename : files) {
			Log.i(TAG, "File name => " + filename);
			InputStream in = null;
			OutputStream out = null;
			try {
				in = assetManager.open("Files/" + filename); // if files resides
																// inside the
																// "Files"
																// directory
																// itself
				out = new FileOutputStream(Environment
						.getExternalStorageDirectory().toString()
						+ "/"
						+ filename);
				copyFile(in, out);
				in.close();
				in = null;
				out.flush();
				out.close();
				out = null;
			} catch (Exception e) {
				Log.i(TAG, e.getMessage());
			}
		}
	}

	private void copyFile(InputStream in, OutputStream out) throws IOException {
		byte[] buffer = new byte[1024];
		int read;
		while ((read = in.read(buffer)) != -1) {
			out.write(buffer, 0, read);
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
}
