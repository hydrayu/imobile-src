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
     * �ѧO�ثe�O�_�bResume���A�C
     */
    private boolean gResumed = false;
    /**
     * �ѧO�ثe�O�_�b�i�g�J���Ҧ��C
     */
    private boolean gWriteMode = false;
    /**
     * �аO��Log�d�߮ɱM�Ϊ����ҡC
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
        
        // ���o�ӳ]�ƹw�]���L�u�P���˸m
        gNfcAdapter = NfcAdapter.getDefaultAdapter(this);
        if (gNfcAdapter == null) {
            Toast.makeText(this, "���䴩NFC�P���\��I", Toast.LENGTH_SHORT).show();
            this.finish();
            return;
        }
        
        // ���oEditText�PButton�A�åB���U�������ƥ�
        findViewById(R.id.write_tag).setOnClickListener(this.gTagWriter);        
        gNote = (EditText)findViewById(R.id.note);
        /**Sophie**/
        //gNote.setText("0;i-Mobile\nSophie LIU\nProject Manager\n3F #8 Alley 15 Lane 120 Sec. 1 Neihu Road, Neihu District, Taipei City 114, Taiwan\n[+886]2-2656-2015#222;[+886]2-2656-2013;[+886]9-1281-7077\nsophie.liu@i-mobile.com.tw\nhttp://www.i-mobile.com.tw/\n");
        /**Bryant**/
        gNote.setText("0;i-Mobile\nBryant CHEN\nGeneral Manager\n3F #8 Alley 15 Lane 120 Sec. 1 Neihu Road, Neihu District, Taipei City 114, Taiwan\n[+886]2-2656-2015#202;[+886]2-2656-2013;[+886]9-7505-1027 /9-6816-9456\nBryant.chen@i-mobile.com.tw\nhttp://www.i-mobile.com.tw/\n");
        /**Ronald**/
//        gNote.setText("0;i-Mobile\nRonald HSU\nVice President OEM /ODM Division\n3F #8 Alley 15 Lane 120 Sec. 1 Neihu Road, Neihu District, Taipei City 114, Taiwan\n[+886]2-2656-2015;[+886]2-2656-2013;[+886]9-5260-3272\nRonald.hsu@i-mobile.com.tw\nhttp://www.i-mobile.com.tw/\n");
        gNote.addTextChangedListener(gTextWatcher);
        
        // ���U����Activity�t�d�B�z�Ҧ������쪺NFC Intents�C
        gNfcPendingIntent = PendingIntent.getActivity(this, 0,
        // ���w��Activity�����ε{�������̤W�hActivity
                new Intent(this, getClass()).addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP), 0);
        
        // �إ߭n�B�z��Intent Filter�t�d�B�z�Ӧ�Tag��p2p�洫����ơC
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
        // �B�z��Android�t�ΰe�X���ε{���B�z��intent filter���e
        if (NfcAdapter.ACTION_NDEF_DISCOVERED.equals(getIntent().getAction())) {
            // ���oNdefMessage
            NdefMessage [] messages = getNdefMessages(getIntent());
            // ���o��ڪ����e
            byte [] payload = messages[0].getRecords()[0].getPayload();
            setNoteBody(new String(payload));
            // ���U�e�X��intent����L���B�z��H
            setIntent(new Intent());
        }
        // �Ұʫe���Ҧ����Nfc intent�B�z
        enableNdefExchangeMode();
    }
    
    @Override
    protected void onPause()
    {
        super.onPause();
        gResumed = false;
        // �ѩ�NfcAdapter�Ұʫe���Ҧ��N�۹��O��h���q�O�A�n�O�o�����C
        gNfcAdapter.disableForegroundNdefPush(this);
    }
    
    @Override
    protected void onNewIntent(Intent intent)
    {
        // �мg��Intent�Ω�ɮ��p�G���s��Intent�i�J�ɡA�i�HĲ�o���ƥ���ȡC
        // NDEF exchange mode
        if (!gWriteMode && NfcAdapter.ACTION_NDEF_DISCOVERED.equals(intent.getAction())) {
            NdefMessage [] msgs = getNdefMessages(intent);
            promptForContent(msgs[0]);
        }
        
        // �ʴ��즳���wACTION�i�J�A�N��n�g�J��Ʀ�Tag���C
        // Tag writing mode
        if (gWriteMode && NfcAdapter.ACTION_TAG_DISCOVERED.equals(intent.getAction())) {
            Tag detectedTag = intent.getParcelableExtra(NfcAdapter.EXTRA_TAG);
            writeTag(getNoteAsNdef(), detectedTag);
        }
    }
    
    /**
     * ���oIntent����J��NdefMessage�C
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
     * �N��J�����e�নNdefMessage�C
     * @return
     */
    private NdefMessage getNoteAsNdef()
    {
        byte [] textBytes = gNote.getText().toString().getBytes();
        NdefRecord textRecord = new NdefRecord(NdefRecord.TNF_MIME_MEDIA, "text/plain".getBytes(), new byte [] {}, textBytes);
        return new NdefMessage(new NdefRecord [] {textRecord});
    }
    
    /**
     * �Ұ�Ndef�洫��ƼҦ��C
     */
    private void enableNdefExchangeMode()
    {
        // ��NfcAdatper�Ұʫe��Push��Ʀ�Tag�����ε{���C
        gNfcAdapter.enableForegroundNdefPush(MainActivity.this, getNoteAsNdef());
        
        // ��NfcAdapter�Ұʯ���b�e���Ҧ��U�i��intent filter��dispatch�C
        gNfcAdapter.enableForegroundDispatch(this, gNfcPendingIntent, gNdefExchangeFilters, null);
    }
    
    private void disableNdefExchangeMode()
    {
        gNfcAdapter.disableForegroundNdefPush(this);
        gNfcAdapter.disableForegroundDispatch(this);
    }
    
    /**
     * �Ұ�Tag�g�J�Ҧ��A���U������Intent Filter�ӫe���Ҧ���ť�O�_��Tag�i�J���T���C
     */
    private void enableTagWriteMode()
    {
        gWriteMode = true;
        IntentFilter tagDetected = new IntentFilter(NfcAdapter.ACTION_TAG_DISCOVERED);
        gWriteTagFilters = new IntentFilter [] {tagDetected};
        gNfcAdapter.enableForegroundDispatch(this, gNfcPendingIntent, gWriteTagFilters, null);
    }
    
    /**
     * ����Tag�g�J�Ҧ��A�����e���Ҧ����ʴ��C
     */
    private void disableTagWriteMode()
    {
        gWriteMode = false;
        gNfcAdapter.disableForegroundDispatch(this);
    }
    
    /**
     * ���ε{���ɮ���Ndef Message�A�߰ݥΤ�O�_�n���N�ثe�e���������C
     * 
     * @param msg
     */
    private void promptForContent(final NdefMessage msg)
    {
        new AlertDialog.Builder(this).setTitle("�O�_���N�{�b�����e?").setPositiveButton("�O", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface arg0, int arg1)
            {
                String body = new String(msg.getRecords()[0].getPayload());
                setNoteBody(body);
            }
        }).setNegativeButton("�_", new DialogInterface.OnClickListener() {
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
            // �����������Intent�A�ǳƼg�J��Ʀ�tag�F
            disableNdefExchangeMode();
            // �Ұʼg�JTag�Ҧ��A�ʴ��O�_��Tag�i�J
            enableTagWriteMode();
            // ��ܹ�ܮءA�i���NTag�Τ���a�񥻾���NFC�P����
            new AlertDialog.Builder(MainActivity.this)
                            .setTitle("Touch tag to write")
                            .setOnCancelListener(new DialogInterface.OnCancelListener() {
                @Override
                public void onCancel(DialogInterface dialog)
                {
                    // �b�����Ҧ��U�A�������ʰ���Tag�ǳƼg�J���Ҧ��A�A�Ұʵ��ݸ�ƥ洫���Ҧ��C
                    // ����g�JTag�Ҧ��A�N��w��Tag�i�J
                    disableTagWriteMode();
                    // �Ұʸ�ƥ洫
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
            // �p�G�O�bResume�����A�U�A��s�觹��A�Ұʫe���o�G�T�����\��C
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
