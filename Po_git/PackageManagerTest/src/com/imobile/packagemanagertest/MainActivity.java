package com.imobile.packagemanagertest;

import java.util.ArrayList;
import java.util.List;

import ProcessInfo.ProcessInfo;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Debug;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

public class MainActivity extends Activity {

	private static String TAG = "ProcessInfo";
	private static final int KILL_PORCESS = 1;
	private static final int SEARCH_RUNNING_APP = 2;

	private ActivityManager mActivityManager = null;
	// ProcessInfo Model类 用来保存所有进程信息
	private List<ProcessInfo> processInfoList = null;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		// test1();
		// kill_me();
		// getRunningAppProcessInfo();
		getRunningAppProcessInfo_Po();

	}

	private void getRunningAppProcessInfo_Po() {

		mActivityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);

		processInfoList = new ArrayList<ProcessInfo>();

		List<ActivityManager.RunningAppProcessInfo> appProcessList = mActivityManager
				.getRunningAppProcesses();

		for (ActivityManager.RunningAppProcessInfo appProcessInfo : appProcessList) {

			int pid = appProcessInfo.pid;

			int uid = appProcessInfo.uid;

			String processName = appProcessInfo.processName;

			int[] myMempid = new int[] { pid };

			Debug.MemoryInfo[] memoryInfo = mActivityManager
					.getProcessMemoryInfo(myMempid);

			int memSize = memoryInfo[0].dalvikPrivateDirty;

			// Log.i(TAG, "processName: " + processName + "  pid: " + pid
			// + " uid:" + uid + " memorySize is -->" + memSize + "kb");

			// find nfc package name
			if (processName.equals("com.android.nfc")) {
				// if (processName.equals("com.imobile.packagemanagertest")) {

				ProcessInfo processInfo = new ProcessInfo();
				processInfo.setPid(pid);
				processInfo.setUid(uid);
				processInfo.setMemSize(memSize);
				processInfo.setPocessName(processName);
				processInfoList.add(processInfo);

				String[] packageList = appProcessInfo.pkgList;
				Log.i(TAG, "process id is " + pid + " has "
						+ packageList.length);
				// for (String pkg : packageList) {
				// Log.i(TAG, "packageName " + pkg + " in process id is -->"
				// + pid);
				// }

				// kill nfc pid
				android.os.Process.killProcess(pid);
				break;
			}

		}
	}

	private void getRunningAppProcessInfo() {

		mActivityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
		// ProcessInfo Model类 用来保存所有进程信息
		processInfoList = new ArrayList<ProcessInfo>();

		// 通过调用ActivityManager的getRunningAppProcesses()方法获得系统里所有正在运行的进程
		List<ActivityManager.RunningAppProcessInfo> appProcessList = mActivityManager
				.getRunningAppProcesses();

		for (ActivityManager.RunningAppProcessInfo appProcessInfo : appProcessList) {
			// 进程ID号
			int pid = appProcessInfo.pid;
			// 用户ID 类似于Linux的权限不同，ID也就不同 比如 root等
			int uid = appProcessInfo.uid;
			// 进程名，默认是包名或者由属性android：process=""指定
			String processName = appProcessInfo.processName;
			// 获得该进程占用的内存
			int[] myMempid = new int[] { pid };
			// 此MemoryInfo位于android.os.Debug.MemoryInfo包中，用来统计进程的内存信息
			Debug.MemoryInfo[] memoryInfo = mActivityManager
					.getProcessMemoryInfo(myMempid);
			// 获取进程占内存用信息 kb单位
			int memSize = memoryInfo[0].dalvikPrivateDirty;

			Log.i(TAG, "processName: " + processName + "  pid: " + pid
					+ " uid:" + uid + " memorySize is -->" + memSize + "kb");
			// 构造一个ProcessInfo对象
			ProcessInfo processInfo = new ProcessInfo();
			processInfo.setPid(pid);
			processInfo.setUid(uid);
			processInfo.setMemSize(memSize);
			processInfo.setPocessName(processName);
			processInfoList.add(processInfo);

			// 获得每个进程里运行的应用程序(包),即每个应用程序的包名
			String[] packageList = appProcessInfo.pkgList;
			Log.i(TAG, "process id is " + pid + "has " + packageList.length);
			for (String pkg : packageList) {
				Log.i(TAG, "packageName " + pkg + " in process id is -->" + pid);
			}
		}
	}

	public void kill_me() {

		ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);

		List<RunningAppProcessInfo> procInfo = activityManager
				.getRunningAppProcesses();
		// for (int i = 0; i < procInfo.size(); i++) {
		// Log.d("TAG" + i,
		// procInfo.get(i).processName + " pid:" + procInfo.get(i).pid
		// + " importance: " + procInfo.get(i).importance
		// + " reason: "
		// + procInfo.get(i).importanceReasonCode);
		// // First I display all processes into the log
		// }
		for (int i = 0; i < procInfo.size(); i++) {
			RunningAppProcessInfo process = procInfo.get(i);
			int importance = process.importance;
			int pid = process.pid;
			String name = process.processName;
			if (name.equals("manager.main")) {
				// I dont want to kill this application
				continue;
			}
			if (importance == RunningAppProcessInfo.IMPORTANCE_SERVICE) {
				// From what I have read about importances at android
				// developers, I asume that I can safely kill everithing except
				// for services, am I right?
				Log.d("TAG", "task " + name + " pid: " + pid
						+ " has importance: " + importance + " WILL NOT KILL");
				continue;
			}
			Log.d("TAG", "[Po 1]task " + name + " pid: " + pid
					+ " has importance: " + importance + " WILL KILL");
			android.os.Process.killProcess(procInfo.get(i).pid);
		}
		procInfo = activityManager.getRunningAppProcesses();
		// I get a new list with running tasks
		// for (int i = 0; i < procInfo.size(); i++) {
		// Log.d("TAG" + i,
		// procInfo.get(i).processName + " pid:" + procInfo.get(i).pid
		// + " importance: " + procInfo.get(i).importance
		// + " reason: "
		// + procInfo.get(i).importanceReasonCode);
		// }
	}

	public void test1() {
		final PackageManager pm = getPackageManager();
		// get a list of installed apps.
		List<ApplicationInfo> packages = pm
				.getInstalledApplications(PackageManager.GET_META_DATA);

		for (ApplicationInfo packageInfo : packages) {
			// Log.d("TAG", "Installed package :" + packageInfo.packageName);
			// Log.d("TAG", "Launch Activity :" +
			// pm.getLaunchIntentForPackage(packageInfo.packageName));
			if (packageInfo.packageName.equals("com.android.nfc")) {
				Log.d("TAG", "equals :" + packageInfo.packageName);

				// ActivityManager mActivityManager =
				// (ActivityManager)this.getSystemService(Context.ACTIVITY_SERVICE);
				// mActivityManager.killBackgroundProcesses(packageInfo.packageName);

			}
		}
		// the getLaunchIntentForPackage returns an intent that you can use with
		// startActivity()

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
