package corba.sample_b;
//
import java.util.*;
import TimeZone.*;
//
public class Timezone_service extends Interface_TimeZonePOA {
	private Calendar cal;
	private TimeZone timezone_current;

	public Timezone_service() {
		super();

		timezone_current = TimeZone.getDefault();
		cal = Calendar.getInstance(timezone_current);
		// create an object from Calendar

	}
	//
	public String get_date() {
		cal = Calendar.getInstance(timezone_current);
		// create a new instance	
		String msg =
			"date:"
				+ (cal.get(cal.MONTH) + 1)
				+ "/"
				+ cal.get(cal.DAY_OF_MONTH)
				+ "/"
				+ cal.get(cal.YEAR);
		return msg;
	}
	//
	public String get_help() {
		return "\n**Server TimeZone**\nmethods:\n help: shows all commands\n get_ids: return the TimeZone know\n get_time: returns the current time of server based in the current TimeZone\n get_date: gets the current date of the server based in the current TimeZone\n set_timezone <TimeZone>: sets the timezone \n";
	}
	//
	public String get_ids() {
		String msg = "TimeZones: \n";
		String[] st = TimeZone.getAvailableIDs();
		for (int i = 0; i < st.length; i++) {
			msg = msg + st[i] + "\n";
		}
		return msg;
	}
	//
	public String get_time() {
		cal = Calendar.getInstance(timezone_current);
		// create a new instance	
		String msg =
			"time:"
				+ cal.get(cal.HOUR_OF_DAY)
				+ ":"
				+ cal.get(cal.MINUTE)
				+ ":"
				+ cal.get(cal.SECOND)
				+ ":"
				+ cal.get(cal.MILLISECOND);
		return msg;
	}
	//
	public String set_timezone(String timezone) {
		String[] st = TimeZone.getAvailableIDs();
		String msg = " ";
		boolean know = false;
		for (int i = 0; i < st.length; i++) {
			if (st[i].equals(timezone))
				know = true;
		}
		if (know) {
			timezone_current = TimeZone.getTimeZone(timezone);
			msg = "timezone setted to " + timezone;
		} else
			msg = "unknow TimeZone:" + timezone;
		return msg;
	}
}
