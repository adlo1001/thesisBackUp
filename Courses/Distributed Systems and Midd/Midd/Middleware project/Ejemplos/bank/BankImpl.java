import MyBank.*;
import java.util.*;

public class BankImpl extends BankPOA {
  
	private List<Account> accounts;
	
	public BankImpl() {
		accounts = new ArrayList<Account>();
	}
  
	//--------------------------------------------------------------------------
	public Account createNewAccount(String name,String address) {
		try {
			int number = accounts.size();
			
			AccountImpl accImpl = 
				new AccountImpl(name, address, number);
			
			System.out.println(_poa().getClass());
			
			org.omg.CORBA.Object ref = 
				_poa().servant_to_reference(accImpl);
			
			Account acc = AccountHelper.narrow(ref);      
			accounts.add(acc);
			return acc;
		} catch(Exception e) { e.printStackTrace(); return null; }
	}
  
	//--------------------------------------------------------------------------
	public Account getAccount (int number) {
		return accounts.get(number);
	}
  
}
