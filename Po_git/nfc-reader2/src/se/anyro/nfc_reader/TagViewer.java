/*
 * Copyright (C) 2010 The Android Open Source Project
 * Copyright (C) 2011 Adam Nyb瓣ck
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package se.anyro.nfc_reader;

import java.nio.charset.Charset;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Locale;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.PendingIntent;
import android.content.DialogInterface;
import android.content.Intent;
import android.media.MediaPlayer;
import android.nfc.NdefMessage;
import android.nfc.NdefRecord;
import android.nfc.NfcAdapter;
import android.nfc.Tag;
import android.nfc.tech.MifareClassic;
import android.nfc.tech.MifareUltralight;
import android.os.Bundle;
import android.os.Parcelable;
import android.provider.Settings;
import android.util.Log;
import android.widget.EditText;
import android.widget.Toast;

/**
 * An {@link Activity} which handles a broadcast of a new tag that the device
 * just discovered.
 * 
 * 格式注意
 * 輸入卡片時最少要有7個欄位
 * 1. 公司名稱
 * P.S 此欄位較特殊，需要用;符號來存取(因為起始字串都有怪怪的字)
 *     比如說:;公司名稱
 *      
 * 2. 姓名
 * 3. 職務
 * 4. 地址
 * 5. 聯絡方式(電話、傳真、手機)
 *    P.S 此欄位較特殊，需要用;符號來存取
 *       比如說:電話;傳真;手機
 * 6. E-Mail
 * 7. 備註
 * 
 */
public class TagViewer extends Activity {

	private static final DateFormat TIME_FORMAT = SimpleDateFormat
			.getDateTimeInstance();
	private EditText Po_ET1, Po_ET2, Po_ET3, Po_ET4, Po_ET5_1, Po_ET5_2,
			Po_ET5_3, Po_ET6, Po_ET7;
	private String NFC_Data = "";
	private NfcAdapter mAdapter;
	private PendingIntent mPendingIntent;
	private NdefMessage mNdefPushMessage;

	private AlertDialog mDialog;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.po_layout2);
		Po_findid();
		resolveIntent(getIntent());

		mDialog = new AlertDialog.Builder(this).setNeutralButton("Ok", null)
				.create();

		mAdapter = NfcAdapter.getDefaultAdapter(this);
		if (mAdapter == null) {
			showMessage(R.string.error, R.string.no_nfc);
			finish();
			return;
		}

		mPendingIntent = PendingIntent.getActivity(this, 0, new Intent(this,
				getClass()).addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP), 0);
		mNdefPushMessage = new NdefMessage(new NdefRecord[] { newTextRecord(
				"Message from NFC Reader :-)", Locale.ENGLISH, true) });
	}

	private void showMessage(int title, int message) {
		mDialog.setTitle(title);
		mDialog.setMessage(getText(message));
		mDialog.show();
	}

	private NdefRecord newTextRecord(String text, Locale locale,
			boolean encodeInUtf8) {
		byte[] langBytes = locale.getLanguage().getBytes(
				Charset.forName("US-ASCII"));

		Charset utfEncoding = encodeInUtf8 ? Charset.forName("UTF-8") : Charset
				.forName("UTF-16");
		byte[] textBytes = text.getBytes(utfEncoding);

		int utfBit = encodeInUtf8 ? 0 : (1 << 7);
		char status = (char) (utfBit + langBytes.length);

		byte[] data = new byte[1 + langBytes.length + textBytes.length];
		data[0] = (byte) status;
		System.arraycopy(langBytes, 0, data, 1, langBytes.length);
		System.arraycopy(textBytes, 0, data, 1 + langBytes.length,
				textBytes.length);

		return new NdefRecord(NdefRecord.TNF_WELL_KNOWN, NdefRecord.RTD_TEXT,
				new byte[0], data);
	}

	@Override
	protected void onResume() {
		super.onResume();
		if (mAdapter != null) {
			if (!mAdapter.isEnabled()) {
				showWirelessSettingsDialog();
			}
			mAdapter.enableForegroundDispatch(this, mPendingIntent, null, null);
			mAdapter.enableForegroundNdefPush(this, mNdefPushMessage);
		}
	}

	@Override
	protected void onPause() {
		super.onPause();
		if (mAdapter != null) {
			mAdapter.disableForegroundDispatch(this);
			mAdapter.disableForegroundNdefPush(this);
		}
	}

	private void showWirelessSettingsDialog() {
		AlertDialog.Builder builder = new AlertDialog.Builder(this);
		builder.setMessage(R.string.nfc_disabled);
		builder.setPositiveButton(android.R.string.ok,
				new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialogInterface, int i) {
						Intent intent = new Intent(
								Settings.ACTION_WIRELESS_SETTINGS);
						startActivity(intent);
					}
				});
		builder.setNegativeButton(android.R.string.cancel,
				new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialogInterface, int i) {
						finish();
					}
				});
		builder.create().show();
		return;
	}

	private void resolveIntent(Intent intent) {
		Log.d("Po", "Po2");
		String action = intent.getAction();
		if (NfcAdapter.ACTION_TAG_DISCOVERED.equals(action)
				|| NfcAdapter.ACTION_TECH_DISCOVERED.equals(action)
				|| NfcAdapter.ACTION_NDEF_DISCOVERED.equals(action)) {
			Parcelable[] rawMsgs = intent
					.getParcelableArrayExtra(NfcAdapter.EXTRA_NDEF_MESSAGES);
			NdefMessage[] msgs = null;
			if (rawMsgs != null) {
				Log.d("Po", "Po if");
				msgs = new NdefMessage[rawMsgs.length];
				for (int i = 0; i < rawMsgs.length; i++) {
					msgs[i] = (NdefMessage) rawMsgs[i];
				}
			} else {
				Log.d("Po", "Po else");
				// Unknown tag type
				// byte[] empty = new byte[0];
				// byte[] id = intent.getByteArrayExtra(NfcAdapter.EXTRA_ID);
				// Parcelable tag = intent
				// .getParcelableExtra(NfcAdapter.EXTRA_TAG);
				// byte[] payload = dumpTagData(tag).getBytes();
				// NdefRecord record = new NdefRecord(NdefRecord.TNF_UNKNOWN,
				// empty, id, payload);
				// NdefMessage msg = new NdefMessage(new NdefRecord[] { record
				// });
				// msgs = new NdefMessage[] { msg };
				Toast("Unknown tag type");
			}
			// Setup the views
			buildTagViews(msgs);
			Po_Nfc_Analysis();
		}
	}

	private String dumpTagData(Parcelable p) {
		Log.d("Po", "Po3");
		StringBuilder sb = new StringBuilder();
		Tag tag = (Tag) p;
		byte[] id = tag.getId();
		sb.append("Tag ID (hex): ").append(getHex(id)).append("\n");
		sb.append("Tag ID (dec): ").append(getDec(id)).append("\n");
		sb.append("ID (reversed): ").append(getReversed(id)).append("\n");

		String prefix = "android.nfc.tech.";
		sb.append("Technologies: ");
		for (String tech : tag.getTechList()) {
			sb.append(tech.substring(prefix.length()));
			sb.append(", ");
		}
		sb.delete(sb.length() - 2, sb.length());
		for (String tech : tag.getTechList()) {
			if (tech.equals(MifareClassic.class.getName())) {
				sb.append('\n');
				MifareClassic mifareTag = MifareClassic.get(tag);
				String type = "Unknown";
				switch (mifareTag.getType()) {
				case MifareClassic.TYPE_CLASSIC:
					type = "Classic";
					break;
				case MifareClassic.TYPE_PLUS:
					type = "Plus";
					break;
				case MifareClassic.TYPE_PRO:
					type = "Pro";
					break;
				}
				sb.append("Mifare Classic type: ");
				sb.append(type);
				sb.append('\n');

				sb.append("Mifare size: ");
				sb.append(mifareTag.getSize() + " bytes");
				sb.append('\n');

				sb.append("Mifare sectors: ");
				sb.append(mifareTag.getSectorCount());
				sb.append('\n');

				sb.append("Mifare blocks: ");
				sb.append(mifareTag.getBlockCount());
			}

			if (tech.equals(MifareUltralight.class.getName())) {
				sb.append('\n');
				MifareUltralight mifareUlTag = MifareUltralight.get(tag);
				String type = "Unknown";
				switch (mifareUlTag.getType()) {
				case MifareUltralight.TYPE_ULTRALIGHT:
					type = "Ultralight";
					break;
				case MifareUltralight.TYPE_ULTRALIGHT_C:
					type = "Ultralight C";
					break;
				}
				sb.append("Mifare Ultralight type: ");
				sb.append(type);
			}
		}

		Log.d("Po", "Po...");
		Log.d("Po", "Po" + sb.toString());
		return sb.toString();
	}

	private String getHex(byte[] bytes) {
		StringBuilder sb = new StringBuilder();
		for (int i = bytes.length - 1; i >= 0; --i) {
			int b = bytes[i] & 0xff;
			if (b < 0x10)
				sb.append('0');
			sb.append(Integer.toHexString(b));
			if (i > 0) {
				sb.append(" ");
			}
		}
		return sb.toString();
	}

	private long getDec(byte[] bytes) {
		long result = 0;
		long factor = 1;
		for (int i = 0; i < bytes.length; ++i) {
			long value = bytes[i] & 0xffl;
			result += value * factor;
			factor *= 256l;
		}
		return result;
	}

	private long getReversed(byte[] bytes) {
		long result = 0;
		long factor = 1;
		for (int i = bytes.length - 1; i >= 0; --i) {
			long value = bytes[i] & 0xffl;
			result += value * factor;
			factor *= 256l;
		}
		return result;
	}

	void buildTagViews(NdefMessage[] msgs) {
		if (msgs == null || msgs.length == 0) {
			return;
		}
		if (msgs[0] != null) {
			String result = "";
			byte[] payload = msgs[0].getRecords()[0].getPayload();
			for (int b = 1; b < payload.length; b++) { // skip SOH
				result += (char) payload[b];
			}

			this.NFC_Data = result;
		}

	}

	public void Toast(String str) {
		Toast.makeText(getApplicationContext(), str, Toast.LENGTH_SHORT).show();
	}

	public void Po_Nfc_Analysis() {
		if (this.NFC_Data != "" && this.NFC_Data != null) {
			Log.i("Po", "Po this.NFC_Data = " + this.NFC_Data);
			String[] user = this.NFC_Data.split("\n");
			String[] Array_5 = null;
			// 判斷資料格式(至少要7格欄位)
			if (user.length >= 7) {
				for (int i = 0; i < user.length; i++) {
					Log.i("Po", "Po this.NFC_Data[" + i + "] =" + user[i]);
					if (user[4] != null) {
						Array_5 = user[4].split(";");
					}
				}

				// for(int i = 0 ; i < Array_5.length ;i ++){
				// Log.i("Po", "Po Array_3[" +i+"] ="+ Array_5[i]);
				// }

				// 解決第一行亂噴文字bug
				if (user[0] != null) {
					String[] Array_1;
					Array_1 = user[0].split(";");
					// Log.i("Po", "Po Array_1.length ="+ Array_1.length);
					if (Array_1.length >= 2) {
						this.Po_ET1.setText(Array_1[1]);
					} else {
						Toast("Company Data format is Error");
						if (Array_1.length == 1) {
							Toast("Please increase; sign in front of the text");
						}
					}
				}

				this.Po_ET2.setText(user[1]);
				this.Po_ET3.setText(user[2]);
				this.Po_ET4.setText(user[3]);
				if (Array_5.length >= 3) {
					this.Po_ET5_1.setText(Array_5[0]);
					this.Po_ET5_2.setText(Array_5[1]);
					this.Po_ET5_3.setText(Array_5[2]);
				} else {
					Toast("NFC Data format is Error");
				}
				this.Po_ET6.setText(user[5]);
				this.Po_ET7.setText(user[6]);
				Music_Play();
			} else {
				// 資料不正確
				Toast("All NFC Data format incorrect");
			}

		}

	}

	public void Po_findid() {
		this.Po_ET1 = (EditText) findViewById(R.id.Po_ET1);
		this.Po_ET2 = (EditText) findViewById(R.id.Po_ET2);
		this.Po_ET3 = (EditText) findViewById(R.id.Po_ET3);
		this.Po_ET4 = (EditText) findViewById(R.id.Po_ET4);
		this.Po_ET5_1 = (EditText) findViewById(R.id.Po_ET5_1);
		this.Po_ET5_2 = (EditText) findViewById(R.id.Po_ET5_2);
		this.Po_ET5_3 = (EditText) findViewById(R.id.Po_ET5_3);
		this.Po_ET6 = (EditText) findViewById(R.id.Po_ET6);
		this.Po_ET7 = (EditText) findViewById(R.id.Po_ET7);
	}

	/* 判斷語系的功能 start */
	public boolean isLunarSetting() {
		String language = getLanguageEnv();

		if (language != null
				&& (language.trim().equals("zh-CN") || language.trim().equals(
						"zh-TW")))
			return true;
		else
			return false;
	}

	private String getLanguageEnv() {
		Locale l = Locale.getDefault();
		String language = l.getLanguage();
		String country = l.getCountry().toLowerCase();
		if ("zh".equals(language)) {
			if ("cn".equals(country)) {
				language = "zh-CN";
			} else if ("tw".equals(country)) {
				language = "zh-TW";
			}
		} else if ("pt".equals(language)) {
			if ("br".equals(country)) {
				language = "pt-BR";
			} else if ("pt".equals(country)) {
				language = "pt-PT";
			}
		}
		return language;
	}

	/* 判斷語系的功能 end */

	/*播放聲音的功能  start*/
	public void Music_Play() {
		MediaPlayer mMediaPlayer = MediaPlayer.create(this, R.raw.beep_3);
		mMediaPlayer.setVolume(100f, 100f);
		mMediaPlayer.start();
	}

	/*播放聲音的功能  end*/
	
	@Override
	public void onNewIntent(Intent intent) {
		setIntent(intent);
		resolveIntent(intent);
	}
}
