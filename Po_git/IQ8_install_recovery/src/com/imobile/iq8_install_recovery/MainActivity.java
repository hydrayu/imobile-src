package com.imobile.iq8_install_recovery;

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;

import com.example.com.imobile.iq8_install_recovery.R;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {

	TextView Tv;
	ProgressBar myProgressBar;
	int myProgress = 20;
	final String CHACK_COPY_FILE_PATH[] = { "/storage/sdcard1/IQ8_update.zip",
			"/storage/usbdisk0/IQ8_update.zip", "/mnt/media_rw/sdcard/IQ8_update.zip", "/mnt/media_rw/usbdisk0/IQ8_update.zip" };
	String COPY_FILE_PATH = null;
	/**
	 * /sdcard/recovery_IQ8.sh
	 */
	String IQ8_recovery_sh_path = "/sdcard/recovery_IQ8.sh";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		Log.e("Debug", "onCreate()");
		findViewId();
		Tv.setText("check Path...");

		// 檢查/mnt/在不在
		for (int i = 0; i < CHACK_COPY_FILE_PATH.length; i++) {
			// Log.e("Debug", "CHACK_COPY_FILE_PATH[" + i + "]=" +
			// CHACK_COPY_FILE_PATH[i]);

			if (fileIsExists(CHACK_COPY_FILE_PATH[i])) {
				this.COPY_FILE_PATH = CHACK_COPY_FILE_PATH[i];
				Log.e("Debug", "this.COPY_FILE_PATH = " + this.COPY_FILE_PATH);
			}
		}

		// 確認/sdcard/recovery_IQ8.sh是否存在
		if (fileIsExists(IQ8_recovery_sh_path)) {
			delete_dile(IQ8_recovery_sh_path);
		}

		if (this.COPY_FILE_PATH != null) {
			myProgress = 60;
			myProgressBar.setProgress(myProgress);
			Create_File(IQ8_recovery_sh_path);
			// /Run_su("busybox chmod 777 /sdcard/u.sh");
			String Title = "Ready to copy files!";
			String Msg = "Copy process takes several minutes, the file copy process do not close the program";
			check_copy_dialog(Title, Msg);

		} else {

			Tv.setText("Did not find IQ8_update.zip\nPlease plug the USB or SDCard");
			String Title = "Error!";
			String Msg = "Did not find IQ8_update.zip\nPlease plug the USB or SDCard";
			show_dialog(Title, Msg);

		}
	}

	private Runnable updateTimer = new Runnable() {
		public void run() {

		}
	};

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

	/**
	 * 刪除檔案
	 * 
	 * @param path
	 *            (路徑)
	 */
	public void delete_dile(String path) {
		File file = new File(path);
		boolean deleted = file.delete();
	}

	/***
	 * 創建檔案
	 * 
	 * @param str
	 *            (路徑)
	 */
	public void Create_File(String str) {
		Log.e("Debug", "Create_File()");
		try {
			String data01 = "busybox mkdir -p /data/recovery_IQ8\n";
			String data02 = "busybox cp " + COPY_FILE_PATH
					+ " /data/recovery_IQ8/\n";
			// 建立FileOutputStream物件，路徑為SD卡中的output.txt
			FileOutputStream output = new FileOutputStream(str);

			output.write(data01.getBytes());
			output.write(data02.getBytes());
			output.close();
			Log.e("Debug", "Successfully Create_File()");
			Toast("Successfully write File");
		} catch (Exception e) {
			Log.e("Debug", "Fails Create_File()");
			Toast("write File Fails");
			e.printStackTrace();
		}
	}

	public void Toast(String str) {
		Toast.makeText(this, str, Toast.LENGTH_SHORT).show();
	}

	@Override
	protected void onDestroy() {
		int pid = android.os.Process.myPid();
		android.os.Process.killProcess(pid);
		super.onDestroy();
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

	/***
	 * 
	 * @param Title
	 * @param Msg
	 */
	public void check_copy_dialog(String Title, String Msg) {
		AlertDialog.Builder dialog = new AlertDialog.Builder(this);
		dialog.setTitle(Title);
		dialog.setMessage(Msg);
		dialog.setIcon(android.R.drawable.ic_dialog_alert);
		dialog.setCancelable(false);
		dialog.setPositiveButton("OK", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int which) {
				// 按下PositiveButton要做的事
				myProgress = 100;
				myProgressBar.setProgress(myProgress);
				Run_su("busybox rm /data/recovery_IQ8.sh; busybox mv /sdcard/recovery_IQ8.sh /data/recovery_IQ8.sh; busybox chmod 777 /data/recovery_IQ8.sh ; ./data/recovery_IQ8.sh");
				// Move_File("/sdcard/command", "/system/command");

				Tv.setText("OK, Please shutdown!");
				String Title = "Please shutdown!";
				String Msg = "Recovery setting is OK\nPlease turn off the machine and then into recovery mode";
				show_dialog(Title, Msg);
			}
		});

		dialog.show();

	}

	/***
	 * 檢查檔案
	 */
	public boolean fileIsExists(String Path) {
		File f = new File(Path);
		if (!f.exists()) {
			return false;
		}
		return true;
	}

	public void findViewId() {
		Tv = (TextView) findViewById(R.id.Po_tv);
		myProgressBar = (ProgressBar) findViewById(R.id.progressbar);
		myProgressBar.setProgress(myProgress);
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
