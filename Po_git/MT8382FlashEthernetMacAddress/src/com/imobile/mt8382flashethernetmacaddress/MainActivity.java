package com.imobile.mt8382flashethernetmacaddress;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStreamWriter;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.ooieueioo.potestjni.PoJNITtest;

public class MainActivity extends Activity {

	private TextView Po_TV1;
	private LinearLayout Po_Llayout1;
	private EditText Po_ET1, Po_ET2, Po_ET3, Po_ET4, Po_ET5, Po_ET6;
	final String Po_debug = "Po_debug";

	static {
		System.loadLibrary("PoTestJNI");
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		findViewId();
		Check_Path("Flash_Ethernet_Mac");
		Show_Mac_Status();

	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	/**
	 * findView by Id
	 */
	public void findViewId() {
		this.Po_TV1 = (TextView) findViewById(R.id.Po_TV1);
		this.Po_TV1.setTextSize(50);
		this.Po_Llayout1 = (LinearLayout) findViewById(R.id.Po_Llayout1);
		this.Po_Llayout1.setVisibility(View.GONE);
		this.Po_ET1 = (EditText) findViewById(R.id.Po_ET1);
		this.Po_ET2 = (EditText) findViewById(R.id.Po_ET2);
		this.Po_ET3 = (EditText) findViewById(R.id.Po_ET3);
		this.Po_ET4 = (EditText) findViewById(R.id.Po_ET4);
		this.Po_ET5 = (EditText) findViewById(R.id.Po_ET5);
		this.Po_ET6 = (EditText) findViewById(R.id.Po_ET6);
	}

	/**
	 * 按鈕Show_Mac_Address
	 * 
	 * @param view
	 */
	public void Show_Mac_Address(View view) {
		// Toast("Show_Mac_Address");
		Po_Llayout1.setVisibility(View.GONE);
		Check_Path("Flash_Ethernet_Mac");
		Show_Mac_Status();
	}

	/**
	 * 按鈕Clear_Mac_Address 清除Mac
	 * 
	 * @param view
	 */
	public void Clear_Mac_Address(View view) {
		// Toast("Clear_Mac_Address");
		Po_Llayout1.setVisibility(View.GONE);
		//** through su **/
		//Run_su("cd /system/ethtool/ ;/system/bin/sh /system/ethtool/ee9wj eth0 /system/ethtool/eeclear.txt");
		
		//** through JNI **/
		// echo ClearMac > /proc/eth0 
		String Clean_status = PoJNITtest.WriteProc("/proc/eth0", "ClearMac");

		if (Clean_status.equals("oo")) {
			Toast("Clear Mac success");
		}else{
			Toast("Clear Mac fail");
		}
		
		//delay 2 sec
		delay_via_thread(3000);
		
		Check_Path("Flash_Ethernet_Mac");
		Show_Mac_Status();
	}
	
	/*****
	 * can delay
	 * example: Thread.sleep(1000);
	 * @param delay_time
	 */
	private void delay_via_thread(int delay_time){
		int DelatTime = delay_time;
		try{
		    // delay 1 second
		    Thread.sleep(DelatTime);
		        
		} catch(InterruptedException e){
		    e.printStackTrace();
		        
		}
	}

	/**
	 * 按鈕Flish_Mac_Address
	 * 
	 * @param view
	 */
	public void Flish_Mac_Address(View view) {
		// Toast("Flish_Mac_Address");
		Po_Llayout1.setVisibility(View.VISIBLE);
	}

	/**
	 * 按鈕Flish
	 * 
	 * @param view
	 */
	public void Flish(View view) {
		// Toast("Flish");

		// Toast(Check_PoET_Data());
		String New_Mac = Check_PoET_Data();
		// Mac正確才能寫入
		if (New_Mac != null) {
			String My_Mac = New_Mac
					+ "0x51 0x55 0x46 0x0a 0x21 0x96 0x00 0x00 0xfc 0x41\n"
					+ "0x10 0x22 0x21 0x32 0x3a 0x0a 0x5a 0x00 0xff 0xff 0xff 0xff 0xff 0xff 0xff 0xff\n"
					+ "0x22 0x03 0x44 0x00 0x41 0x00 0x56 0x00 0x49 0x00 0x43 0x00 0x4f 0x00 0x4d 0x00\n"
					+ "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00\n"
					+ "0x00 0x00 0x32 0x03 0x44 0x00 0x4d 0x00 0x39 0x00 0x36 0x00 0x32 0x00 0x31 0x00\n"
					+ "0x20 0x00 0x55 0x00 0x53 0x00 0x42 0x00 0x20 0x00 0x54 0x00 0x6f 0x00 0x20 0x00\n"
					+ "0x46 0x00 0x61 0x00 0x73 0x00 0x74 0x00 0x20 0x00 0x45 0x00 0x74 0x00 0x68 0x00\n"
					+ "0x65 0x00 0x72 0x00 0x0a 0x03 0x30 0x00 0x30 0x00 0x30 0x00 0x33 0x00 0xc3 0x61\n";
			Create_File("/sdcard/Flash_Ethernet_Mac/eedata.txt", My_Mac);

			//Run_su("cd /system/ethtool/ ;/system/bin/sh /system/ethtool/ee9wj eth0 /system/ethtool/eeclear.txt;/system/bin/sh /system/ethtool/ee9wj eth0 /sdcard/Flash_Ethernet_Mac/eedata.txt");
			//** through JNI **/
			// echo ClearMac > /proc/eth0 
			String Flash_status = PoJNITtest.WriteProc("/proc/eth0", "FlashMacSD");

			if (Flash_status.equals("oo")) {
				Toast("Flash Mac success");
			}else{
				Toast("Flash Mac fail");
			}
			
			//delay 2 sec
			delay_via_thread(3000);

			Show_Mac_Status();
			// Run_su("cd /system/ethtool/ ;/system/bin/sh /system/ethtool/ee9wj eth0 /system/ethtool/eedata.txt");
		}
	}

	/**
	 * 確認EditText格式
	 * 
	 * @return
	 */
	private String Check_PoET_Data() {
		int Check_ETData_Ok = 0;
		String New_Mac_Address = "";
		EditText[] tmp = { this.Po_ET1, this.Po_ET2, this.Po_ET3, this.Po_ET4,
				this.Po_ET5, this.Po_ET6 };
		for (int i = 0; i < tmp.length; i++) {
			int tmp_num;
			if ("".equals(tmp[i].getText().toString().trim())) {
				// edittext輸入為空白或無輸入
				tmp_num = i + 1;
				Toast("Columns " + tmp_num + " is null\nPlease input number");
			} else {
				tmp_num = i + 1;
				// edittext有輸入
				if (tmp[i].length() == 2) {
					// Toast("Flish");
					Check_ETData_Ok++;
					New_Mac_Address += "0x" + tmp[i].getText().toString() + " ";
				} else {
					// Toast("length "+ tmp[i].length());
					Toast("Columns " + tmp_num + " data is error");
				}

			}
		}
		if (Check_ETData_Ok == 6) {
			// 資料正確
			Toast("Data ok");
			return New_Mac_Address;
		} else {
			// 資料不正確
			Toast("Data error");
		}

		// 資料一定不正確
		return null;
	}

	public void Show_Mac_Status() {

		// 1. through su get LAN MAC
		Run_su("cd /system/ethtool/ ;/system/bin/sh /system/ethtool/ee9wj eth0 | busybox grep 0x0000> /system/MacStatus.txt");

		// 2. through su run echo ShowMac > /proc/eth0 command, via JNI command
		// 會造成 system reboot
		Run_su("echo ShowMac > /proc/eth0");

		// 3. through JNI read Mac
		String str = PoJNITtest.ReadMacAddress();
		Log.d(Po_debug, "str =" + str);

		String My_Mac_Addres = "Mac Address: ";

		// 拆解字串
		My_Mac_Addres += Dismantling(str);

		this.Po_TV1.setText(My_Mac_Addres);
	}

	private String Dismantling(String str) {
		String Mac_Addres = "";
		String Str = str;

		// 1. 清除空白

		String[] token = Str.split("\\s+");
		// for (int i = 0; i < token.length; i++) {
		//
		// Log.e("Po_debug", "token[" + i + "]= " + token[i]);
		//
		// }

		// 2. 檢查是否符合格式
		if (token.length == 2) {
			// Log.e("Debug", "token[1] = " + token[1]);
			// 3. 切出所有mac號碼
			String tmp = token[1];
			if (tmp.length() == 12) {
				String Mac1 = tmp.substring(0, 2);
				String Mac2 = tmp.substring(2, 4);
				String Mac3 = tmp.substring(4, 6);
				String Mac4 = tmp.substring(6, 8);
				String Mac5 = tmp.substring(8, 10);
				String Mac6 = tmp.substring(10, 12);
				Mac_Addres += Mac1 + ":" + Mac2 + ":" + Mac3 + ":" + Mac4 + ":"
						+ Mac5 + ":" + Mac6;
				Log.d("Po_debug", "Mac_Addres = " + Mac_Addres);

			} else {
				Mac_Addres += "Not Read Mac Address";
				return Mac_Addres;
			}

		} else {
			Mac_Addres += "Not Read Mac Address";
		}
		return Mac_Addres;
	}

	/**
	 * 確定SD卡可讀寫
	 */
	public void Check_Path(String name) {
		if (Environment.getExternalStorageState()// 確定SD卡可讀寫
				.equals(Environment.MEDIA_MOUNTED)) {

			File sdFile = android.os.Environment.getExternalStorageDirectory();
			String path = sdFile.getPath() + File.separator + name;

			File dirFile = new File(path);

			if (!dirFile.exists()) {// 如果資料夾不存在

				dirFile.mkdir();// 建立資料夾
			}
		}
	}

	public void Run_CMD(String str) {
		// String cmd = "ls";
		String cmd = str;
		try {
			Process ps = Runtime.getRuntime().exec(cmd);
			Log.e("Debug", "Run CMD " + loadStream(ps.getInputStream()));
		} catch (IOException ioe) {
			Log.e("Debug", "Run CMD XX");
			ioe.printStackTrace();
		}

	}

	/**
	 * read an input-stream into a String
	 * 
	 * @param in
	 * @return
	 * @throws IOException
	 */
	static String loadStream(InputStream in) throws IOException {
		int ptr = 0;
		in = new BufferedInputStream(in);
		StringBuffer buffer = new StringBuffer();
		while ((ptr = in.read()) != -1) {
			buffer.append((char) ptr);
		}
		return buffer.toString();
	}

	public void Create_File(String file_path, String str) {
		Log.e("Debug", "Create_File()");
		try {
			String data01 = str;
			// String data02 = "busybox cp " + COPY_FILE_PATH + " /data/\n";
			// 建立FileOutputStream物件，路徑為SD卡中的output.txt
			FileOutputStream output = new FileOutputStream(file_path);

			output.write(data01.getBytes());
			// output.write(data02.getBytes());
			output.close();
			Log.e("Debug", "Successfully Create_File()");
			Toast("Successfully write /sdcard/command");
		} catch (Exception e) {
			Log.e("Debug", "Fails Create_File()");
			Toast("write File Fails");
			e.printStackTrace();
		}
	}

	public void Run_su(String str) {
		Runtime ex = Runtime.getRuntime();
		String cmdBecomeSu = "su";
		String script = str;
		Log.e("Debug", "Run_su()");
		try {
			Process runsum = ex.exec(cmdBecomeSu);
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

	/**
	 * 
	 * @param call
	 * Toast
	 */
	public void Toast(String str) {
		Toast.makeText(this, str, Toast.LENGTH_SHORT).show();
	}

}
