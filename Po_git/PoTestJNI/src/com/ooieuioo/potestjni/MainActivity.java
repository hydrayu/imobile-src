package com.ooieuioo.potestjni;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.TextView;

public class MainActivity extends Activity {

	LinearLayout Po_LLay1 = null;
	static {
		System.loadLibrary("PoTestJNI");
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		find_id();
		call_JNI_function();

	}

	private void find_id() {
		Po_LLay1 = (LinearLayout) findViewById(R.id.Po_LLay1);
	}

	private void call_JNI_function() {
		// call HelloWorld()
		String str = PoJNITtest.HelloWorld();
		TextView tv = new TextView(this);
		tv.setText(str);
		tv.setTextSize(24);
		Po_LLay1.addView(tv);

		// call TransportStringToC()
		String str2 = PoJNITtest.TransportStringToC("Java_Send");
		TextView tv2 = new TextView(this);
		tv2.setText(str2);
		tv2.setTextSize(24);
		Po_LLay1.addView(tv2);

		// call TransportIntToC()
		int Po_Java_int = PoJNITtest.TransportIntToC(20);
		TextView tv3 = new TextView(this);
		tv3.setText("JNI return number = " + Po_Java_int);
		tv3.setTextSize(24);
		Po_LLay1.addView(tv3);

		// show origin function
		int[] Po_int_array = { 1, 2, 3 };
		TextView tv4 = new TextView(this);
		tv4.setText("origin array = {" + Po_int_array[0] + ","
				+ Po_int_array[1] + "," + Po_int_array[2] + "}");
		tv4.setTextSize(24);
		Po_LLay1.addView(tv4);
		// call TransportIntArrayToC()
		int[] Po_IntArray_fromJNI =  PoJNITtest.TransportIntArrayToC(Po_int_array);
		TextView tv5 = new TextView(this);
		tv5.setText("updata array = {" + Po_IntArray_fromJNI[0] + ","
				+ Po_IntArray_fromJNI[1] + "," + Po_IntArray_fromJNI[2] + "}");
		tv5.setTextSize(24);
		Po_LLay1.addView(tv5);
		
		// call TransportBooleanToC()
		boolean Po_Java_boolean = PoJNITtest.TransportBooleanToC(false);
		TextView tv6 = new TextView(this);
		tv6.setText("JNI return boolean = " + Po_Java_boolean);
		tv6.setTextSize(24);
		Po_LLay1.addView(tv6);
		
		// call PoTryExec()
		String str3 = PoJNITtest.ReadProc("Po");
		TextView tv7 = new TextView(this);
		tv7.setText("Read /proc/Po_value = "+str3);
		tv7.setTextSize(24);
		Po_LLay1.addView(tv7);
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
