package com.ooieueioo.ctrl_power_widget;

import android.app.PendingIntent;
import android.appwidget.AppWidgetManager;
import android.appwidget.AppWidgetProvider;
import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.content.Intent;
import android.net.wifi.WifiManager;
import android.provider.Settings;
import android.util.Log;
import android.widget.RemoteViews;

import com.ooieueioo.ctrl_power_widget.adrian_bl.ApnICS;
import com.ooieueioo.ctrl_power_widget.adrian_bl.ApnLegacy;

public class MyWidgetProvider extends AppWidgetProvider {

	private static final String CLICK_ACTION = "myCustomAction";
	private static final String Po_btn1_wifi = "Po_WiFi";
	private static final String Po_btn2_BT = "Po_BT";
	private static final String Po_btn3_GPS = "Po_GPS";
	private static final String Po_btn4_3G = "Po_3G";
	private static boolean wifi_status = false;
	private static boolean BT_status = false;
	private static boolean GPS_status = false;
	private static boolean net_3G_status = false;
	private Context Po_GPS_use_context = null;
	private Context Po_context = null;
	private int []Po_appWidgetId;
	private AppWidgetManager Po_app_manager; 

	private void Po_init_parameter(Context context){
		
		// read wifi status
		WifiManager wifiManager = (WifiManager) context
				.getSystemService(Context.WIFI_SERVICE);
		if (!wifiManager.isWifiEnabled()) {
			this.wifi_status = false;
		} else {
			this.wifi_status = true;
		}

		// read bluetooth status
		BluetoothAdapter mBluetoothAdapter = BluetoothAdapter
				.getDefaultAdapter();
		if (!mBluetoothAdapter.isEnabled()) {
			this.BT_status = false;
		} else {
			this.BT_status = true;
		}

		// read GPS status
		String provider = Settings.Secure.getString(
				context.getContentResolver(),
				Settings.Secure.LOCATION_PROVIDERS_ALLOWED);
		this.GPS_status = provider.contains("gps");

		//read 3G status
		String status = new Boolean(getApnStatus(context)).toString();
		if(status.equals("true")){
			this.net_3G_status = true;
		}else{
			this.net_3G_status = false;
		}
		
	}
	
	@Override
	public void onReceive(Context context, Intent intent) {
		super.onReceive(context, intent);

		String action = intent.getAction();

		System.out.println("onReveice action: " + action);
		Log.d("Po", "action=" + action);

		if (CLICK_ACTION.equals(action)) {
			System.out.println("clicked");
			// Toast.makeText(context, Read_Carrier(),
			// Toast.LENGTH_LONG).show();

		}

		if (Po_btn1_wifi.equals(action)) {
			toggle_WiFi(context);
		}

		// if (Po_btn1_wifi.equals(intent.getAction())) {
		// toggle_WiFi(context);
		// }

		if (Po_btn2_BT.equals(action)) {
			toggle_BT(context);
		}

		if (Po_btn3_GPS.equals(action)) {
			toggle_GPS(context);
		}

		if (Po_btn4_3G.equals(action)) {
			// toggle_GPS(context);
			//TelephonyManager telephonyManager = (TelephonyManager) context
			//		.getSystemService(Context.TELEPHONY_SERVICE);
			//Log.d("Po", "telephonyManager.getDataState()="+telephonyManager.getDataState());
			toggleApnStatus(context);
		}
	}

	/*****
	 * toggle_WiFi
	 * 
	 * @param context
	 *            on/off WiFi
	 */
	public void toggle_WiFi(Context context) {
		WifiManager wifiManager = (WifiManager) context
				.getSystemService(Context.WIFI_SERVICE);
		if (!wifiManager.isWifiEnabled()) {
			wifiManager.setWifiEnabled(true);
			this.wifi_status = true;
		} else {
			wifiManager.setWifiEnabled(false);
			this.wifi_status = false;
		}
		Log.d("Po", "toggleWiFi() wifi_status=" + this.wifi_status);
		// System.out.println("toggleWiFi() wifi_status=" + this.wifi_status);
	}

	/***
	 * toggle_BT
	 * 
	 * @param context
	 *            on/off BuleTooth
	 */
	public void toggle_BT(Context context) {
		BluetoothAdapter mBluetoothAdapter = BluetoothAdapter
				.getDefaultAdapter();
		if (!mBluetoothAdapter.isEnabled()) {
			mBluetoothAdapter.enable();
			this.BT_status = true;
		} else {
			mBluetoothAdapter.disable();
			this.BT_status = false;
		}
		Log.d("Po", "toggle_BT() BT_status=" + this.BT_status);
	}

	/****
	 * toggle_GPS
	 * 
	 * @param context
	 *            on/of GPS
	 */
	public void toggle_GPS(Context context) {
		//open GPS setting dialog
		Intent intent = new Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS);
	    intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        context.startActivity(intent);
		
        this.Po_GPS_use_context = context;
        
        
		// start thread
		 Thread thread = new Thread(new Po_Read_GPS_status());
		 thread.start();
        
        /*
        Po_GPS_provider = Settings.Secure.getString(
				context.getContentResolver(),
				Settings.Secure.LOCATION_PROVIDERS_ALLOWED);
		this.GPS_status = Po_GPS_provider.contains("gps");
		if (this.GPS_status == true) {
			Log.d("Po", "toggle_GPS() GPS_status=true");
		} else {
			Intent poke = new Intent();
			poke.setClassName("com.android.settings",
					"com.android.settings.widget.SettingsAppWidgetProvider");
			poke.addCategory(Intent.CATEGORY_ALTERNATIVE);
			poke.setData(Uri.parse("3"));
			context.sendBroadcast(poke);
			Log.d("Po", "toggle_GPS() GPS_status=flase");
		}
         */
		// Log.d("Po", "toggle_GPS() GPS_status=" + this.GPS_status);
	}
	
	//*************** copy from https://github.com/adrian-bl/APN-Switch start***************//
	
	/*
	 * Toggle APN on/off
	*/
	private void toggleApnStatus(Context ctx) {
		setApnStatus(ctx, getApnStatus(ctx) ? false : true);
		
		//check 3G status
		String status = new Boolean(getApnStatus(ctx)).toString();
		if(status.equals("true")){
			this.net_3G_status = true;
		}else{
			this.net_3G_status = false;
		}			
				
		log("toggleApnStatus: APN-enabled: " + this.net_3G_status);
	}
	
	/*
	 * Enables APN if 'on' is TRUE
	*/
	private void setApnStatus(Context ctx, boolean on) {
		if(isICS()) { (new ApnICS()).setApnStatus(ctx,on);    }
		else        { (new ApnLegacy()).setApnStatus(ctx,on); }
	}
	
	/*
	 * Returns current APN-Status : true == apn is enabled
	*/
	private boolean getApnStatus(Context ctx) {
		if(isICS()) { return (new ApnICS()).getApnStatus(ctx);    }
		else        { return (new ApnLegacy()).getApnStatus(ctx); }
	}
	
	private void log(String lmsg) {
//		android.util.Log.d("ApnSwitch INFO: ", lmsg);
		//Po modify
		android.util.Log.d("Po", lmsg);
	}
	
	/*
	 * Returns TRUE if we should use ApnICS(); -> ICS is SDK_INT 14
	*/
	private boolean isICS() {
		return ( android.os.Build.VERSION.SDK_INT >= 14 ? true : false );
	}

	//*************** copy from https://github.com/adrian-bl/APN-Switch END***************//
	
	@Override
	public void onEnabled(Context context) {
		super.onEnabled(context);

		System.out.println("onEnabled");

	}

	@Override
	public void onUpdate(Context context, AppWidgetManager appWidgetManager,
			int[] appWidgetIds) {
		
		this.Po_context = context;
		this.Po_appWidgetId = appWidgetIds;
		this.Po_app_manager = appWidgetManager;
		
		Log.d("Po", "onUpdate");
		System.out.println("on-update widget");

		for (int widgetId : appWidgetIds) {
			RemoteViews remoteViews = new RemoteViews(context.getPackageName(),
					R.layout.widget_layout);

			Intent intent = new Intent(context, MyWidgetProvider.class);

			intent.setAction(CLICK_ACTION);
			intent.putExtra(AppWidgetManager.EXTRA_APPWIDGET_IDS, appWidgetIds);

			PendingIntent pendingIntent = PendingIntent.getBroadcast(context,
					0, intent, 0);

			remoteViews.setOnClickPendingIntent(R.id.WidgetLayout,
					pendingIntent);
			// Po add button
			remoteViews.setOnClickPendingIntent(R.id.Po_bt1,
					getPendingSelfIntent(context, Po_btn1_wifi));
			remoteViews.setOnClickPendingIntent(R.id.Po_bt2,
					getPendingSelfIntent(context, Po_btn2_BT));
			remoteViews.setOnClickPendingIntent(R.id.Po_bt3,
					getPendingSelfIntent(context, Po_btn3_GPS));
			remoteViews.setOnClickPendingIntent(R.id.Po_bt4,
					getPendingSelfIntent(context, Po_btn4_3G));

			appWidgetManager.updateAppWidget(widgetId, remoteViews);
		}
		
		// init
		Po_init_parameter(context);
		
		// ±Ò°Êthread
		 Thread thread = new Thread(new update_UI());
		 thread.start();
	}

	private PendingIntent getPendingSelfIntent(Context context, String action) {
		// An explicit intent directed at the current class (the "self").
		Intent intent = new Intent(context, getClass());
		intent.setAction(action);
		return PendingIntent.getBroadcast(context, 0, intent, 0);
	}

	public class Po_Read_GPS_status implements Runnable {

		@Override
		public void run() {
			// TODO Auto-generated method stub
			int Po_read_GPS_start = 0;
			int Po_read_GPS_end =30;
			while(Po_read_GPS_start++ < Po_read_GPS_end){
				try {
					Thread.sleep(1000);
					String provider = Settings.Secure.getString(
							Po_GPS_use_context.getContentResolver(),
							Settings.Secure.LOCATION_PROVIDERS_ALLOWED);
					GPS_status = provider.contains("gps");
					Log.d("Po", "GPS_status="+GPS_status + "  time="+Po_read_GPS_start);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		
	}
	
	public class update_UI implements Runnable {
		@Override
		public void run() {
			
			int i = 0;
			while (true) {

				try {
					Thread.sleep(1000);

					// if ((i == 2) || (i == 5) || (i==10) || (i == 20) || (i ==
					// 30)){
					 RemoteViews views = new RemoteViews(
					 Po_context.getPackageName(), R.layout.widget_layout);
					 
					 // read wifi status
					 if(wifi_status){
						 //true
						 views.setImageViewResource(R.id.Po_bt1, R.drawable.ic_appwidget_settings_wifi_on_holo);
						 views.setImageViewResource(R.id.Po_IV1_below, R.drawable.blue);
					 }else{
						 views.setImageViewResource(R.id.Po_bt1, R.drawable.ic_appwidget_settings_wifi_off_holo);
						 views.setImageViewResource(R.id.Po_IV1_below, R.drawable.white);
					 }
					 
					 // read bluetooth status
					 if(BT_status){
						 //true
						 views.setImageViewResource(R.id.Po_bt2, R.drawable.ic_appwidget_settings_bluetooth_on_holo);
						 views.setImageViewResource(R.id.Po_IV2_below, R.drawable.blue);
					 }else{
						 views.setImageViewResource(R.id.Po_bt2, R.drawable.ic_appwidget_settings_bluetooth_off_holo);
						 views.setImageViewResource(R.id.Po_IV2_below, R.drawable.white);
					 }
					 
					 // read GPS status
					 if(GPS_status){
						 //true
						 views.setImageViewResource(R.id.Po_bt3, R.drawable.ic_appwidget_settings_gps_on_holo);
						 views.setImageViewResource(R.id.Po_IV3_below, R.drawable.blue);
					 }else{
						 views.setImageViewResource(R.id.Po_bt3, R.drawable.ic_appwidget_settings_gps_off_holo);
						 views.setImageViewResource(R.id.Po_IV3_below, R.drawable.white);
					 }

					 // read 3G status
					 if(net_3G_status){
						 //true
						 views.setImageViewResource(R.id.Po_bt4, R.drawable.ic_network_signal_4);
						 views.setImageViewResource(R.id.Po_IV4_below, R.drawable.blue);
					 }else{
						 views.setImageViewResource(R.id.Po_bt4, R.drawable.ic_network_signal_0);
						 views.setImageViewResource(R.id.Po_IV4_below, R.drawable.white);
					 }
					 
					 Po_app_manager.updateAppWidget(Po_appWidgetId, views);

					
					i++;
					Log.d("Po", "update_UI() time=" + i);
					
					if (i == 60 * 60 *30) {
						//if 30 min set i =0 
						i = 0;
					}
					
					Log.d("Po", "update_UI() wifi_status=" + wifi_status);
					Log.d("Po", "update_UI() BT_status=" + BT_status);
					Log.d("Po", "update_UI() GPS_status=" + GPS_status);
					Log.d("Po", "update_UI() net_3G_status=" + net_3G_status);
					
					
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
					Log.d("Po", "InterruptedException");
				}

			}
		}
	}

}
