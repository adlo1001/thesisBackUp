import Finance.*;

public class AccountImpl extends AccountPOA {
  private AccountDetails acc;

  public AccountImpl(String name, String address, int number) {
    acc = new AccountDetails(name, address, number, 0);
  }

  public void deposit(double amount) {
    acc.balance+=amount;
  }

  public void withdraw(double amount) throws InsufficientFunds {
    if(acc.balance<amount) throw new InsufficientFunds();
    acc.balance-=amount;
  }

  public AccountDetails details() {
    return acc;
  }
}
