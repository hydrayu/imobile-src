package com.ooieueioo.read_from_keyboard;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.app.Activity;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.Menu;
import android.widget.TextView;

public class MainActivity extends Activity {

	private static final Pattern KEYCODE_PATTERN = Pattern
			.compile("KEYCODE_(\\w)");

	private TextView mTextView;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		mTextView = (TextView) findViewById(R.id.textView1);
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		String key = KeyEvent.keyCodeToString(keyCode);
		// key will be something like "KEYCODE_A" - extract the "A"

		// use pattern to convert int keycode to some character
		Matcher matcher = KEYCODE_PATTERN.matcher(key);
		if (matcher.matches()) {
			// append character to textview
			mTextView.append(matcher.group(1));
		}
		// let the default implementation handle the event
		return super.onKeyDown(keyCode, event);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
