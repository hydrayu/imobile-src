package com.ooieueioo.flash_imt8_imei;

import java.io.FileOutputStream;
import java.io.OutputStreamWriter;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Color;
import android.os.Bundle;
import android.provider.Settings.Secure;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {

	int not_exit_APP = 0;
	int exit_APP = 1;
	String identifier = null;
	TextView Po_show_imei_number, Po_show_imei;
	EditText Po_et_show_imei_number;
	String Po_IMEI;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		show_imei();
		Po_findViewById();

	}

	public void Po_Btn_flash_imei_number(View view) {
		
		this.Po_IMEI = Po_et_show_imei_number.getText().toString();
		if (this.Po_IMEI.length() == 15) {
			Create_File("/sdcard/imei.sh", this.Po_IMEI);
			Run_su("busybox chmod 777 /sdcard/imei.sh ;sleep 1; busybox msh /sdcard/imei.sh; sleep 1;busybox cp /sdcard/imei.sh /system/local_script/flash_imei.sh;sleep 1; busybox rm -r /sdcard/imei.sh");
			show_dialog("Successfully", "Please reboot IMT8", exit_APP);
			
		} else {
			
			show_dialog("Format Error", "Please enter 15 characters", not_exit_APP);
		}

	}
	/**
	 * onDestroy()
	 */
	@Override
	protected void onDestroy() {
		int pid = android.os.Process.myPid();
		android.os.Process.killProcess(pid);
		super.onDestroy();
	}
	
	public void show_dialog(String Title, String Msg, int exit_status) {
		AlertDialog.Builder dialog = new AlertDialog.Builder(this);
		dialog.setTitle(Title);
		dialog.setMessage(Msg);
		dialog.setIcon(android.R.drawable.ic_dialog_alert);
		dialog.setCancelable(false);
		if(exit_status == not_exit_APP){
			dialog.setPositiveButton("OK", new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int which) {
					Toast("Format Error");
				}
			});
			
		}else{//exit_status =exit_APP
			dialog.setPositiveButton("OK", new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int which) {
					// 按下PositiveButton要做的事
					Toast("APP is exit");
					onDestroy();
				}
			});
		}		

		dialog.show();

	}

	/**
	 * 
	 * @param call
	 *            Create_File(String str, String imei_number) input path and
	 *            imei number
	 */
	public void Create_File(String str, String imei_number) {
		Log.e("Debug", "Create_File()");
		try {
			// String data01 =
			// "echo  'AT+EGMR=1,7,\"123456789012347\"'>/dev/radio/pttycmd1\n";
			String data01 = "echo  'AT+EGMR=1,7,\"" + imei_number
					+ "\"'>/dev/radio/pttycmd1\n";
			// String data02 = "busybox cp " + COPY_FILE_PATH + " /data/\n";
			// 建立FileOutputStream物件，路徑為SD卡中的output.txt
			FileOutputStream output = new FileOutputStream(str);

			output.write(data01.getBytes());
			// output.write(data02.getBytes());
			output.close();
			Log.e("Debug", "Successfully Create_File()");
			Toast("Successfully write /sdcard/" + str);
		} catch (Exception e) {
			Log.e("Debug", "Fails Create_File()");
			Toast("write File Fails");
			e.printStackTrace();
		}
	}

	/**
	 * 
	 * @param call
	 *            Run_su(String str) input command
	 */
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

	private void Po_findViewById() {
		
		Po_show_imei = (TextView) findViewById(R.id.Po_show_imei);
		Po_show_imei_number = (TextView) findViewById(R.id.Po_show_imei_number);

		int text_size = 30;
		Po_show_imei.setTextSize(text_size);
		Po_show_imei_number.setTextSize(text_size);

		Po_show_imei.setText("Your IMEI :");
		if(this.identifier.toString().equals("Invalid")){
			Po_show_imei_number.setTextColor(android.graphics.Color.RED);
		}
		Po_show_imei_number.setText(this.identifier.toString());

		TextView Po_show_input_imei;

		Po_show_input_imei = (TextView) findViewById(R.id.Po_show_input_imei);
		Po_show_input_imei.setTextSize(text_size);
		Po_show_input_imei.setText("Input you imei :");

		Po_et_show_imei_number = (EditText) findViewById(R.id.Po_et_show_imei_number);

	}

	private void show_imei() {
		TelephonyManager tm = (TelephonyManager) this
				.getSystemService(Context.TELEPHONY_SERVICE);
		if (tm != null){
			identifier = tm.getDeviceId();
		}
		if (identifier == null || identifier.length() == 0){
			identifier = Secure.getString(this.getContentResolver(),
					Secure.ANDROID_ID);
			identifier = "Invalid";
			
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
