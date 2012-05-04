class Buffer {
    String msg;

    public synchronized void put(String msg) {
	while(msg!=null) wait();
	this.msg = msg;
	notifyAll();
    }

    public synchronized String get() {
	String m;
	while(msg==null) wait();
	m = msg;
	msg = null;
	notifyAll();
	return m;
    }
}



