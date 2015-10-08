package com.ooieuioo.potestjni;


public class PoJNITtest{
	static public native String HelloWorld();
	
	static public native String TransportStringToC(String str);
	
	static public native int TransportIntToC(int number);
	
	//int array function
	static public native int[] TransportIntArrayToC(int[] int_Array);
	
	static public native boolean TransportBooleanToC(boolean bool);
	
	static public native String ReadProc(String Po_value);
}