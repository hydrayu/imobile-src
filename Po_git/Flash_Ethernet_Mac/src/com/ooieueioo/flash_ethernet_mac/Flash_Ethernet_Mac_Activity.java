package com.ooieueioo.flash_ethernet_mac;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
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

public class Flash_Ethernet_Mac_Activity extends Activity {

	private TextView Po_TV1;
	private LinearLayout Po_Llayout1;
	private EditText Po_ET1, Po_ET2, Po_ET3, Po_ET4, Po_ET5, Po_ET6;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_flash__ethernet__mac_);
		// Run_CMD("/system/bin/sh /system/ethtool/ee9wj eth0");

		findViewId();
		Check_Path("Flash_Ethernet_Mac");
		Show_Mac_Status();

		// Run_su("cd /system/ethtool/ ;/system/bin/sh /system/ethtool/ee9wj eth0 /system/ethtool/eeclear.txt");
		// Run_su("cd /system/ethtool/ ;/system/bin/sh /system/ethtool/ee9wj eth0 /system/ethtool/eedata.txt");

		// Create_File("/sdcard/flash_eth.sh",
		// "cd /system/ethtool\n/system/bin/sh /system/ethtool/ee9wj eth0 /system/ethtool/eedata.txt\n");
		// Run_su("busybox chmod 777 /sdcard/flash_eth.sh ; busybox msh /sdcard/flash_eth.sh");

	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.flash__ethernet__mac_, menu);
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
		Run_su("cd /system/ethtool/ ;/system/bin/sh /system/ethtool/ee9wj eth0 /system/ethtool/eeclear.txt");
		Check_Path("Flash_Ethernet_Mac");
		Show_Mac_Status();
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

			Run_su("cd /system/ethtool/ ;/system/bin/sh /system/ethtool/ee9wj eth0 /system/ethtool/eeclear.txt;/system/bin/sh /system/ethtool/ee9wj eth0 /sdcard/Flash_Ethernet_Mac/eedata.txt");
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
		Run_su("cd /system/ethtool/ ;/system/bin/sh /system/ethtool/ee9wj eth0 > /sdcard/Flash_Ethernet_Mac/status");
		File dir = Environment.getExternalStorageDirectory();

		// 開啟或建立該目錄底下檔名為 "test.txt" 的檔案
		File inFile = new File(dir, "/Flash_Ethernet_Mac/status");

		// 讀取 /data/data/com.myapp/test.txt 檔案內容
		String data = readFromFile(inFile);
		String My_Mac_Addres = "Mac Address: ";
		// 拆解字串
		My_Mac_Addres += Dismantling(data);

		this.Po_TV1.setText(My_Mac_Addres);
	}

	private String Dismantling(String str) {
		String Mac_Addres = "";
		String Str = str;
		String[] token = Str.split("0x0000:");
		// 1. 切出0x0000:這行
		// for (int i = 0; i < token.length; i++) {
		//
		// Log.e("Debug", "token[" + i + "]= " + token[i]);
		//
		// }
		// 2. 檢查是否符合格式
		if (token.length == 2) {
			// Log.e("Debug", "token[1] = " + token[1]);
			// 3. 切出所有mac號碼
			String tmp = token[1];
			String[] token2 = tmp.split(" ");
			// for (int i = 0; i < token2.length; i++) {
			//
			// Log.e("Debug", "token[" + i + "]= " + token2[i]);
			//
			// }

			// 4. 檢查mac號碼
			if (token2.length > 6) {
				for (int num = 0; num < 6; num++) {
					Mac_Addres += token2[num] + " ";
				}
				// Log.e("Debug", "Mac_Addres =" + Mac_Addres);
			}

		} else {
			Mac_Addres += "Not Read Mac Address";
		}
		return Mac_Addres;
	}

	private String readFromFile(File fin) {
		StringBuilder data = new StringBuilder();
		BufferedReader reader = null;
		try {
			reader = new BufferedReader(new InputStreamReader(
					new FileInputStream(fin), "utf-8"));
			String line;
			while ((line = reader.readLine()) != null) {
				data.append(line);
			}
		} catch (Exception e) {
			;
		} finally {
			try {
				reader.close();
			} catch (Exception e) {
				;
			}
		}
		return data.toString();
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
				// Toast("Successfully to su");
			}
		} catch (Exception e) {
			Log.e("Debug", "Fails to su");
			Toast("Fails to su");
		}
	}

	/**
	 * 
	 * @param call
	 *            Toast
	 */
	public void Toast(String str) {
		Toast.makeText(this, str, Toast.LENGTH_SHORT).show();
	}

}
