package ProcessInfo;

public class ProcessInfo {

	// 进程id Android规定android.system.uid=1000
	private int pid;
	// 进程所在的用户id ，即该进程是有谁启动的 root/普通用户等
	private int uid;
	// 进程占用的内存大小,单位为kb
	private int memSize;
	// 进程名
	private String processName;

	public ProcessInfo() {
	}

	public int getPid() {
		return pid;
	}

	public void setPid(int pid) {
		this.pid = pid;
	}

	public int getUid() {
		return uid;
	}

	public void setUid(int uid) {
		this.uid = uid;
	}

	public int getMemSize() {
		return memSize;
	}

	public void setMemSize(int memSize) {
		this.memSize = memSize;
	}

	public String getProcessName() {
		return processName;
	}

	public void setPocessName(String processName) {
		this.processName = processName;
	}
}
