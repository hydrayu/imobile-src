package com.ooieueioo.nfc;

import java.io.IOException;

import com.pou.android.NFCProject.R;

import android.nfc.NdefMessage;
import android.nfc.NdefRecord;
import android.nfc.NfcAdapter;
import android.nfc.Tag;
import android.nfc.tech.Ndef;
import android.nfc.tech.NdefFormatable;
import android.os.Bundle;
import android.os.Parcelable;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.PendingIntent;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.IntentFilter.MalformedMimeTypeException;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity
{
    NfcAdapter gNfcAdapter;
    
    /**
     * 識別目前是否在Resume狀態。
     */
    private boolean gResumed = false;
    /**
     * 識別目前是否在可寫入的模式。
     */
    private boolean gWriteMode = false;
    /**
     * 標記為Log查詢時專用的標籤。
     */
    private static final String TAG = "nfcproject";
    
    PendingIntent gNfcPendingIntent;
    IntentFilter [] gNdefExchangeFilters;
    IntentFilter [] gWriteTagFilters;
    
    EditText gNote;
    
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        View linearLayout =  findViewById(R.id.info);
        //LinearLayout layout = (LinearLayout) findViewById(R.id.info);


        TextView valueTV = new TextView(this);
        valueTV.setText("hallo hallo");
        valueTV.setId(5);
        valueTV.setLayoutParams(new LayoutParams(
                LayoutParams.FILL_PARENT,
                LayoutParams.WRAP_CONTENT));

        ((LinearLayout) linearLayout).addView(valueTV);
        
        // 取得該設備預設的無線感應裝置
        gNfcAdapter = NfcAdapter.getDefaultAdapter(this);
        if (gNfcAdapter == null) {
            Toast.makeText(this, "不支援NFC感應功能！", Toast.LENGTH_SHORT).show();
            this.finish();
            return;
        }
        
        // 取得EditText與Button，並且註冊對應的事件
        findViewById(R.id.write_tag).setOnClickListener(this.gTagWriter);        
        gNote = (EditText)findViewById(R.id.note);
        /**Sophie**/
        //gNote.setText("0;i-Mobile\nSophie LIU\nProject Manager\n3F #8 Alley 15 Lane 120 Sec. 1 Neihu Road, Neihu District, Taipei City 114, Taiwan\n[+886]2-2656-2015#222;[+886]2-2656-2013;[+886]9-1281-7077\nsophie.liu@i-mobile.com.tw\nhttp://www.i-mobile.com.tw/\n");
        /**Bryant**/
        gNote.setText("0;i-Mobile\nBryant CHEN\nGeneral Manager\n3F #8 Alley 15 Lane 120 Sec. 1 Neihu Road, Neihu District, Taipei City 114, Taiwan\n[+886]2-2656-2015#202;[+886]2-2656-2013;[+886]9-7505-1027 /9-6816-9456\nBryant.chen@i-mobile.com.tw\nhttp://www.i-mobile.com.tw/\n");
        /**Ronald**/
//        gNote.setText("0;i-Mobile\nRonald HSU\nVice President OEM /ODM Division\n3F #8 Alley 15 Lane 120 Sec. 1 Neihu Road, Neihu District, Taipei City 114, Taiwan\n[+886]2-2656-2015;[+886]2-2656-2013;[+886]9-5260-3272\nRonald.hsu@i-mobile.com.tw\nhttp://www.i-mobile.com.tw/\n");
        gNote.addTextChangedListener(gTextWatcher);
        
        // 註冊讓該Activity負責處理所有接收到的NFC Intents。
        gNfcPendingIntent = PendingIntent.getActivity(this, 0,
        // 指定該Activity為應用程式中的最上層Activity
                new Intent(this, getClass()).addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP), 0);
        
        // 建立要處理的Intent Filter負責處理來自Tag或p2p交換的資料。
        IntentFilter ndefDetected = new IntentFilter(NfcAdapter.ACTION_NDEF_DISCOVERED);
        try {
            ndefDetected.addDataType("text/plain");
        }
        catch (MalformedMimeTypeException e) {}
        gNdefExchangeFilters = new IntentFilter [] {ndefDetected};
    }
    public void Po_findId(){
    	
    }
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu)
    {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
    @Override
    protected void onResume()
    {
        super.onResume();
        gResumed = true;
        // 處理由Android系統送出應用程式處理的intent filter內容
        if (NfcAdapter.ACTION_NDEF_DISCOVERED.equals(getIntent().getAction())) {
            // 取得NdefMessage
            NdefMessage [] messages = getNdefMessages(getIntent());
            // 取得實際的內容
            byte [] payload = messages[0].getRecords()[0].getPayload();
            setNoteBody(new String(payload));
            // 往下送出該intent給其他的處理對象
            setIntent(new Intent());
        }
        // 啟動前景模式支持Nfc intent處理
        enableNdefExchangeMode();
    }
    
    @Override
    protected void onPause()
    {
        super.onPause();
        gResumed = false;
        // 由於NfcAdapter啟動前景模式將相對花費更多的電力，要記得關閉。
        gNfcAdapter.disableForegroundNdefPush(this);
    }
    
    @Override
    protected void onNewIntent(Intent intent)
    {
        // 覆寫該Intent用於補捉如果有新的Intent進入時，可以觸發的事件任務。
        // NDEF exchange mode
        if (!gWriteMode && NfcAdapter.ACTION_NDEF_DISCOVERED.equals(intent.getAction())) {
            NdefMessage [] msgs = getNdefMessages(intent);
            promptForContent(msgs[0]);
        }
        
        // 監測到有指定ACTION進入，代表要寫入資料至Tag中。
        // Tag writing mode
        if (gWriteMode && NfcAdapter.ACTION_TAG_DISCOVERED.equals(intent.getAction())) {
            Tag detectedTag = intent.getParcelableExtra(NfcAdapter.EXTRA_TAG);
            writeTag(getNoteAsNdef(), detectedTag);
        }
    }
    
    /**
     * 取得Intent中放入的NdefMessage。
     * @param intent
     * @return
     */
    NdefMessage [] getNdefMessages(Intent intent)
    {
        // Parse the intent
        NdefMessage [] msgs = null;
        String action = intent.getAction();
        if (NfcAdapter.ACTION_TAG_DISCOVERED.equals(action) || NfcAdapter.ACTION_NDEF_DISCOVERED.equals(action)) {
            Parcelable [] rawMsgs = intent.getParcelableArrayExtra(NfcAdapter.EXTRA_NDEF_MESSAGES);
            if (rawMsgs != null) {
                msgs = new NdefMessage [rawMsgs.length];
                for (int i = 0; i < rawMsgs.length; i++) {
                    msgs[i] = (NdefMessage)rawMsgs[i];
                }
            }
            else {
                // Unknown tag type
                byte [] empty = new byte [] {};
                NdefRecord record = new NdefRecord(NdefRecord.TNF_UNKNOWN, empty, empty, empty);
                NdefMessage msg = new NdefMessage(new NdefRecord [] {record});
                msgs = new NdefMessage [] {msg};
            }
        }
        else {
            Log.d(TAG, "Unknown intent.");
            finish();
        }
        return msgs;
    }
    
    private void setNoteBody(String body)
    {
        Editable text = gNote.getText();
        text.clear();
        text.append(body);
    }
    
    /**
     * 將輸入的內容轉成NdefMessage。
     * @return
     */
    private NdefMessage getNoteAsNdef()
    {
        byte [] textBytes = gNote.getText().toString().getBytes();
        NdefRecord textRecord = new NdefRecord(NdefRecord.TNF_MIME_MEDIA, "text/plain".getBytes(), new byte [] {}, textBytes);
        return new NdefMessage(new NdefRecord [] {textRecord});
    }
    
    /**
     * 啟動Ndef交換資料模式。
     */
    private void enableNdefExchangeMode()
    {
        // 讓NfcAdatper啟動前景Push資料至Tag或應用程式。
        gNfcAdapter.enableForegroundNdefPush(MainActivity.this, getNoteAsNdef());
        
        // 讓NfcAdapter啟動能夠在前景模式下進行intent filter的dispatch。
        gNfcAdapter.enableForegroundDispatch(this, gNfcPendingIntent, gNdefExchangeFilters, null);
    }
    
    private void disableNdefExchangeMode()
    {
        gNfcAdapter.disableForegroundNdefPush(this);
        gNfcAdapter.disableForegroundDispatch(this);
    }
    
    /**
     * 啟動Tag寫入模式，註冊對應的Intent Filter來前景模式監聽是否有Tag進入的訊息。
     */
    private void enableTagWriteMode()
    {
        gWriteMode = true;
        IntentFilter tagDetected = new IntentFilter(NfcAdapter.ACTION_TAG_DISCOVERED);
        gWriteTagFilters = new IntentFilter [] {tagDetected};
        gNfcAdapter.enableForegroundDispatch(this, gNfcPendingIntent, gWriteTagFilters, null);
    }
    
    /**
     * 停止Tag寫入模式，取消前景模式的監測。
     */
    private void disableTagWriteMode()
    {
        gWriteMode = false;
        gNfcAdapter.disableForegroundDispatch(this);
    }
    
    /**
     * 應用程式補捉到Ndef Message，詢問用戶是否要取代目前畫面中的文件。
     * 
     * @param msg
     */
    private void promptForContent(final NdefMessage msg)
    {
        new AlertDialog.Builder(this).setTitle("是否取代現在的內容?").setPositiveButton("是", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface arg0, int arg1)
            {
                String body = new String(msg.getRecords()[0].getPayload());
                setNoteBody(body);
            }
        }).setNegativeButton("否", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface arg0, int arg1)
            {
                
            }
        }).show();
    }
    
    private View.OnClickListener gTagWriter = new View.OnClickListener() {
        
        @Override
        public void onClick(View v)
        {
            // 先停止接收任何的Intent，準備寫入資料至tag；
            disableNdefExchangeMode();
            // 啟動寫入Tag模式，監測是否有Tag進入
            enableTagWriteMode();
            // 顯示對話框，告知將Tag或手機靠近本機的NFC感應區
            new AlertDialog.Builder(MainActivity.this)
                            .setTitle("Touch tag to write")
                            .setOnCancelListener(new DialogInterface.OnCancelListener() {
                @Override
                public void onCancel(DialogInterface dialog)
                {
                    // 在取消模式下，先關閉監偵有Tag準備寫入的模式，再啟動等待資料交換的模式。
                    // 停止寫入Tag模式，代表已有Tag進入
                    disableTagWriteMode();
                    // 啟動資料交換
                    enableNdefExchangeMode();
                }
            }).create().show();
            
        }
    };
    
    private TextWatcher gTextWatcher = new TextWatcher() {

        @Override
        public void onTextChanged(CharSequence arg0, int arg1, int arg2, int arg3) {

        }

        @Override
        public void beforeTextChanged(CharSequence arg0, int arg1, int arg2, int arg3) {

        }

        @Override
        public void afterTextChanged(Editable arg0) {
            // 如果是在Resume的狀態下，當編輯完後，啟動前景發佈訊息的功能。
            if (gResumed) {
                gNfcAdapter.enableForegroundNdefPush(MainActivity.this, getNoteAsNdef());
            }
        }
    };
    
    boolean writeTag(NdefMessage message, Tag tag) {
        int size = message.toByteArray().length;

        try {
            Ndef ndef = Ndef.get(tag);
            if (ndef != null) {
                ndef.connect();

                if (!ndef.isWritable()) {
                    toast("Tag is read-only.");
                    return false;
                }
                if (ndef.getMaxSize() < size) {
                    toast("Tag capacity is " + ndef.getMaxSize() + " bytes, message is " + size
                            + " bytes.");
                    return false;
                }

                ndef.writeNdefMessage(message);
                toast("Wrote message to pre-formatted tag.");
                return true;
            } else {
                NdefFormatable format = NdefFormatable.get(tag);
                if (format != null) {
                    try {
                        format.connect();
                        format.format(message);
                        toast("Formatted tag and wrote message");
                        return true;
                    } catch (IOException e) {
                        toast("Failed to format tag.");
                        return false;
                    }
                } else {
                    toast("Tag doesn't support NDEF.");
                    return false;
                }
            }
        } catch (Exception e) {
            toast("Failed to write tag");
        }

        return false;
    }
    
    private void toast(String text) {
        Toast.makeText(this, text, Toast.LENGTH_SHORT).show();
    }
}
