package com.imobile.iq8settingethernetip;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends Activity {

	private final String TAG = "Po_ETH";
	private int Default_config = 9;
	private int Custom_config = 1;
	private int Read_my_IP = 8;

	// Po_IP_area
	private EditText Po_IP_1, Po_IP_2, Po_IP_3, Po_IP_4;
	// Po_Mask_area
	private EditText Po_Mask_1, Po_Mask_2, Po_Mask_3, Po_Mask_4;
	// Po_Gateway_area
	private EditText Po_GW_1, Po_GW_2, Po_GW_3, Po_GW_4;
	// Po_DNS_area
	private EditText Po_DNS_1, Po_DNS_2, Po_DNS_3, Po_DNS_4;
	// Po_DNS2_area
	private EditText Po_DNS2_1, Po_DNS2_2, Po_DNS2_3, Po_DNS2_4;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		Po_find_id();
	}

	/***
	 * Po_find_id(), this function will find id
	 */
	private void Po_find_id() {

		// find id IP area
		this.Po_IP_1 = (EditText) findViewById(R.id.Po_IP_1);
		this.Po_IP_2 = (EditText) findViewById(R.id.Po_IP_2);
		this.Po_IP_3 = (EditText) findViewById(R.id.Po_IP_3);
		this.Po_IP_4 = (EditText) findViewById(R.id.Po_IP_4);

		// find id Mask area
		this.Po_Mask_1 = (EditText) findViewById(R.id.Po_Mask_1);
		this.Po_Mask_2 = (EditText) findViewById(R.id.Po_Mask_2);
		this.Po_Mask_3 = (EditText) findViewById(R.id.Po_Mask_3);
		this.Po_Mask_4 = (EditText) findViewById(R.id.Po_Mask_4);

		// find id Gateway area
		this.Po_GW_1 = (EditText) findViewById(R.id.Po_GW_1);
		this.Po_GW_2 = (EditText) findViewById(R.id.Po_GW_2);
		this.Po_GW_3 = (EditText) findViewById(R.id.Po_GW_3);
		this.Po_GW_4 = (EditText) findViewById(R.id.Po_GW_4);

		// find id DNS area
		this.Po_DNS_1 = (EditText) findViewById(R.id.Po_DNS_1);
		this.Po_DNS_2 = (EditText) findViewById(R.id.Po_DNS_2);
		this.Po_DNS_3 = (EditText) findViewById(R.id.Po_DNS_3);
		this.Po_DNS_4 = (EditText) findViewById(R.id.Po_DNS_4);

		// find id DNS2 area
		this.Po_DNS2_1 = (EditText) findViewById(R.id.Po_DNS2_1);
		this.Po_DNS2_2 = (EditText) findViewById(R.id.Po_DNS2_2);
		this.Po_DNS2_3 = (EditText) findViewById(R.id.Po_DNS2_3);
		this.Po_DNS2_4 = (EditText) findViewById(R.id.Po_DNS2_4);

	}

	/***
	 * read_current_IP() can read /mnt/shell/emulated/0/IQ8_IP_info, IQ8_IP_info
	 * have IQ8 IP information
	 * 
	 * @return current_IP
	 */
	private String read_current_IP() {
		StringBuilder text = new StringBuilder();
		try {
			File sdcard = Environment.getExternalStorageDirectory();
			File file = new File(sdcard, "IQ8_IP_info");

			BufferedReader br = new BufferedReader(new FileReader(file));
			String line;
			while ((line = br.readLine()) != null) {
				text.append(line);
				// Log.i(TAG, "text : " + text + " : end");
				text.append('\n');
			}
			br.close();
		} catch (IOException e) {
			e.printStackTrace();
			return "X";
		}
		// Log.d(TAG, text.toString());
		return text.toString();
	}

	/***
	 * Po_check_range(), this function can analyze user input number P.S user
	 * input number can not > 255, can not < 0
	 * 
	 * @param tmp
	 *            (user input number)
	 * @return success/error P.S return 0 is error P.S return 1 is success
	 */
	private int Po_check_range(String tmp[]) {
		for (int i = 0; i < tmp.length; i++) {
			int intValue = Integer.parseInt(tmp[i]);
			// Log.i(TAG, "intValue="+intValue);
			if ((intValue > 255) || (intValue < 0)) {
				return 0;
			}
		}
		return 1;
	}

	/**
	 * Read_Mask(), can read user input Mask
	 * 
	 * @return
	 */
	private String Read_Mask() {

		String Mask = "";
		String PoTmp_Mask1, PoTmp_Mask2, PoTmp_Mask3, PoTmp_Mask4;
		PoTmp_Mask1 = this.Po_Mask_1.getText().toString();
		PoTmp_Mask2 = this.Po_Mask_2.getText().toString();
		PoTmp_Mask3 = this.Po_Mask_3.getText().toString();
		PoTmp_Mask4 = this.Po_Mask_4.getText().toString();

		if ((!PoTmp_Mask1.equals("")) & (!PoTmp_Mask2.equals(""))
				& (!PoTmp_Mask3.equals("")) & (!PoTmp_Mask4.equals(""))) {

			// check range
			String tmp[] = { PoTmp_Mask1, PoTmp_Mask2, PoTmp_Mask3, PoTmp_Mask4 };
			int status = Po_check_range(tmp);
			if (status == 1) {
				// success
				Mask = PoTmp_Mask1 + "." + PoTmp_Mask2 + "." + PoTmp_Mask3
						+ "." + PoTmp_Mask4;
			} else {
				return "X";
			}

		} else {
			// Log.i(TAG, "Mask is null");
			return "X";
		}
		Log.i(TAG, "Mask=" + Mask);

		return Mask;
	}

	/**
	 * Read_DNS2(), can read user input DNS2
	 * 
	 * @return DNS2
	 */
	private String Read_DNS2() {

		String DNS2 = "";
		String PoTmp_DNS1, PoTmp_DNS2, PoTmp_DNS3, PoTmp_DNS4;
		PoTmp_DNS1 = this.Po_DNS2_1.getText().toString();
		PoTmp_DNS2 = this.Po_DNS2_2.getText().toString();
		PoTmp_DNS3 = this.Po_DNS2_3.getText().toString();
		PoTmp_DNS4 = this.Po_DNS2_4.getText().toString();

		if ((!PoTmp_DNS1.equals("")) & (!PoTmp_DNS2.equals(""))
				& (!PoTmp_DNS3.equals("")) & (!PoTmp_DNS4.equals(""))) {

			// check range
			String tmp[] = { PoTmp_DNS1, PoTmp_DNS2, PoTmp_DNS3, PoTmp_DNS4 };
			int status = Po_check_range(tmp);
			if (status == 1) {
				// success
				DNS2 = PoTmp_DNS1 + "." + PoTmp_DNS2 + "." + PoTmp_DNS3 + "."
						+ PoTmp_DNS4;
			} else {
				return "X";
			}

		} else {
			// Log.i(TAG, "DNS2 is null");
			return "X";
		}
		Log.i(TAG, "DNS2=" + DNS2);

		return DNS2;
	}

	/***
	 * Read_DNS(), can read user input DNS
	 * 
	 * @return DNS
	 */
	private String Read_DNS() {

		String DNS = "";
		String PoTmp_DNS1, PoTmp_DNS2, PoTmp_DNS3, PoTmp_DNS4;
		PoTmp_DNS1 = this.Po_DNS_1.getText().toString();
		PoTmp_DNS2 = this.Po_DNS_2.getText().toString();
		PoTmp_DNS3 = this.Po_DNS_3.getText().toString();
		PoTmp_DNS4 = this.Po_DNS_4.getText().toString();

		if ((!PoTmp_DNS1.equals("")) & (!PoTmp_DNS2.equals(""))
				& (!PoTmp_DNS3.equals("")) & (!PoTmp_DNS4.equals(""))) {

			// check range
			String tmp[] = { PoTmp_DNS1, PoTmp_DNS2, PoTmp_DNS3, PoTmp_DNS4 };
			int status = Po_check_range(tmp);
			if (status == 1) {
				// success
				DNS = PoTmp_DNS1 + "." + PoTmp_DNS2 + "." + PoTmp_DNS3 + "."
						+ PoTmp_DNS4;
			} else {
				return "X";
			}

		} else {
			// Log.i(TAG, "IP_address is null");
			return "X";
		}
		Log.i(TAG, "DNS=" + DNS);

		return DNS;
	}

	/**
	 * Read user input gateway function
	 * 
	 * @return gateway
	 */
	private String Read_Gateway() {

		String Gateway_address = "";
		String PoTmp_GW1, PoTmp_GW2, PoTmp_GW3, PoTmp_GW4;
		PoTmp_GW1 = this.Po_GW_1.getText().toString();
		PoTmp_GW2 = this.Po_GW_2.getText().toString();
		PoTmp_GW3 = this.Po_GW_3.getText().toString();
		PoTmp_GW4 = this.Po_GW_4.getText().toString();

		if ((!PoTmp_GW1.equals("")) & (!PoTmp_GW2.equals(""))
				& (!PoTmp_GW3.equals("")) & (!PoTmp_GW4.equals(""))) {

			// check range
			String tmp[] = { PoTmp_GW1, PoTmp_GW2, PoTmp_GW3, PoTmp_GW4 };
			int status = Po_check_range(tmp);
			if (status == 1) {
				// success
				Gateway_address = PoTmp_GW1 + "." + PoTmp_GW2 + "." + PoTmp_GW3
						+ "." + PoTmp_GW4;
			} else {
				return "X";
			}

		} else {
			// Log.i(TAG, "IP_address is null");
			return "X";
		}
		Log.i(TAG, "Gateway_address=" + Gateway_address);

		return Gateway_address;
	}

	/***
	 * Read user input IP(if error can return X)
	 * 
	 * @return IP
	 */
	private String Read_IP() {

		String IP_address = "";
		String PoTmp_IP1, PoTmp_IP2, PoTmp_IP3, PoTmp_IP4;
		PoTmp_IP1 = this.Po_IP_1.getText().toString();
		PoTmp_IP2 = this.Po_IP_2.getText().toString();
		PoTmp_IP3 = this.Po_IP_3.getText().toString();
		PoTmp_IP4 = this.Po_IP_4.getText().toString();

		if ((!PoTmp_IP1.equals("")) & (!PoTmp_IP2.equals(""))
				& (!PoTmp_IP3.equals("")) & (!PoTmp_IP4.equals(""))) {

			// check range
			String tmp[] = { PoTmp_IP1, PoTmp_IP2, PoTmp_IP3, PoTmp_IP4 };
			int status = Po_check_range(tmp);
			if (status == 1) {
				// success
				IP_address = PoTmp_IP1 + "." + PoTmp_IP2 + "." + PoTmp_IP3
						+ "." + PoTmp_IP4;
			} else {
				return "X";
			}

		} else {
			// Log.i(TAG, "IP_address is null");
			return "X";
		}
		Log.i(TAG, "IP_address=" + IP_address);

		return IP_address;
	}

	/***
	 * can save user input data to /mnt/shell/emulated/0/IQ8_EthernetIP.sh
	 * 
	 * @param IP
	 * @param Mask
	 * @param Gateway
	 * @param DNS
	 * @param DNS2
	 * @return success/error
	 */
	private int Po_write_file(String IP, String Mask, String Gateway,
			String DNS, String DNS2) {
		try {
			// Path ==> /mnt/shell/emulated/0/
			FileWriter fw = new FileWriter("/sdcard/IQ8_EthernetIP.sh", false);
			BufferedWriter bw = new BufferedWriter(fw); // 將BufferedWeiter與FileWrite物件做連結
			bw.write("#!/system/bin/sh\n");
			bw.write("\n");
			bw.write("     netcfg | grep eth0 | grep DOWN && DOWN=\"1\" || DOWN=\"0\"\n");
			bw.write("     if [ \"$DOWN\" = \"1\" ]; then\n");
			bw.write("         netcfg eth0 dhcp\n");
			bw.write("         sleep 1\n");
			bw.write("     else\n");
			bw.write("         #busybox ifconfig eth0 " + IP + "\n");
			bw.write("         busybox ifconfig eth0 " + IP + " netmask "
					+ Mask + "\n");
			bw.write("     fi\n");
			bw.write("     \n");
			bw.write("     ETH0=`getprop init.svc.dhcpcd_eth0`\n");
			bw.write("     if [ ! \"x$ETH0\" = \"xrunning\" ]; then\n");
			bw.write("         start dhcpcd_eth0\n");
			bw.write("         sleep 5\n");
			bw.write("     fi\n");
			bw.write("     \n");
			bw.write("     ndc resolver setifdns eth0 \"\" " + DNS + " " + DNS2
					+ "\n");
			bw.write("     ndc resolver setdefaultif eth0\n");
			bw.write("     \n");
			bw.write("     route add default gw " + Gateway + " dev eth0\n");
			bw.write("     \n");
			bw.write("     sleep 3");
			bw.newLine();
			bw.close();
		} catch (IOException e) {
			e.printStackTrace();
			return 0;
		}
		return 1;
	}

	@Override
	protected void onDestroy() {
		Log.d(TAG, "exit APP");
		int pid = android.os.Process.myPid();
		android.os.Process.killProcess(pid);
		super.onDestroy();
	}

	/***
	 * 顯示diglog
	 * 
	 * @param Title
	 * @param Msg
	 * @param event
	 */
	public void show_dialog(String Title, String Msg, int Po_event) {
		AlertDialog.Builder dialog = new AlertDialog.Builder(this);
		dialog.setTitle(Title);
		dialog.setMessage(Msg);
		dialog.setIcon(android.R.drawable.ic_dialog_alert);
		dialog.setCancelable(false);

		if (Po_event == this.Default_config) {
			dialog.setIcon(R.drawable.success);
			dialog.setPositiveButton("OK",
					new DialogInterface.OnClickListener() {

						public void onClick(DialogInterface dialog, int which) {

							onDestroy();
						}
					});
		} else if (Po_event == this.Custom_config) {
			dialog.setIcon(R.drawable.success);
			dialog.setPositiveButton("OK",
					new DialogInterface.OnClickListener() {

						public void onClick(DialogInterface dialog, int which) {

							onDestroy();
						}
					});
		} else if (Po_event == this.Read_my_IP) {
			dialog.setIcon(R.drawable.success);
			dialog.setPositiveButton("OK",
					new DialogInterface.OnClickListener() {

						public void onClick(DialogInterface dialog, int which) {

						}
					});
		} else {
			dialog.setIcon(R.drawable.error);
			dialog.setPositiveButton("OK",
					new DialogInterface.OnClickListener() {

						public void onClick(DialogInterface dialog, int which) {

						}
					});
		}

		dialog.show();

	}

	/***
	 * Po_set_default_config is a button, can delete /sdcard/IQ8_EthernetIP.sh
	 * 
	 * @param view
	 */
	public void Po_save_config(View view) {
		Log.d(TAG, "Po_save_config()");

		// check data is null??
		if (this.Read_IP().equals("X")) {
			Toast.makeText(MainActivity.this, "IP Address format is error!",
					Toast.LENGTH_SHORT).show();
		} else if (this.Read_Mask().equals("X")) {
			Toast.makeText(MainActivity.this, "Mask format is error!",
					Toast.LENGTH_SHORT).show();
		} else if (this.Read_Gateway().equals("X")) {
			Toast.makeText(MainActivity.this, "Gateway format is error!",
					Toast.LENGTH_SHORT).show();
		} else if (this.Read_DNS().equals("X")) {
			Toast.makeText(MainActivity.this, "DNS format is error!",
					Toast.LENGTH_SHORT).show();
		} else if (this.Read_DNS2().equals("X")) {
			Toast.makeText(MainActivity.this, "DNS2 format is error!",
					Toast.LENGTH_SHORT).show();
		} else {
			int Po_write_status = Po_write_file(this.Read_IP(),
					this.Read_Mask(), this.Read_Gateway(), this.Read_DNS(),
					this.Read_DNS2());
			if (Po_write_status == 1) {
				show_dialog("Save config", "save is success\nwill exit APP!",
						this.Custom_config);
			} else {
				Toast.makeText(MainActivity.this, "modify IP address error!!",
						Toast.LENGTH_SHORT).show();
			}
		}

	}

	/***
	 * Po_read_current_IP is a button, can read current IP information
	 * 
	 * @param view
	 */
	public void Po_read_current_IP(View view) {
		Log.d(TAG, "Po_read_current_IP()");
		// 1. read current IP
		String Po_tmp_IP_info;
		Po_tmp_IP_info = read_current_IP();

		// 2. check IP information
		if (Po_tmp_IP_info.equals("X")) {
			// Read file error!!
			show_dialog("Error", "read IP information is error!!", 0);
		} else {

			// 3. analyse IP information
			String[] vElement = Po_tmp_IP_info.split(",");

			// Log.d(TAG, vElement.length+"");
			// for( int i = 0; i < vElement.length; i++ ){
			// Log.d(TAG, vElement[i]);
			// }
			// split
			if (vElement.length == 3) {
				String[] tmp;
				String Last_String;

				// 3.1 split IP
				String Po_current_IP = "";
				tmp = vElement[0].split(" ");
				for (int i = 0; i < tmp.length; i++) {
					Po_current_IP += tmp[i];
				}
				tmp = null;

				// 3.2 split Mask
				String Po_current_Mask = "";
				tmp = vElement[1].split(" ");
				for (int i = 0; i < tmp.length; i++) {
					Po_current_Mask += tmp[i];
				}
				tmp = null;

				String Po_current_Gateway = vElement[2];

				// marge String
				// Last_String = "IP address:" + Po_current_IP + "\nMask:"
				// + Po_current_Mask + "\nGateway:" + Po_current_Gateway;
				// Po modify (custom say not show gateway)
				Last_String = "IP address:" + Po_current_IP + "\nMask:"
						+ Po_current_Mask;
				// Log.d(TAG, Po_current_IP);
				// Log.d(TAG, Po_current_Mask);
				// Log.d(TAG, Po_current_Gateway);

				// show dialog
				show_dialog("Current IP information", Last_String,
						this.Read_my_IP);
			} else {
				// Read file error!!
				show_dialog("Error", "read IP information is error!!", 0);
			}
		}

	}

	/***
	 * Po_set_default_config is a button, can delete /sdcard/IQ8_EthernetIP.sh
	 * 
	 * @param view
	 */
	public void Po_set_default_config(View view) {
		Log.d(TAG, "Po_set_default_config()");
		File delete_file = new File("/sdcard/IQ8_EthernetIP.sh");
		// delete file
		delete_file.delete();
		show_dialog("Restore config",
				"Restore IQ8 default is success\nPlease reboot device!",
				this.Default_config);
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
