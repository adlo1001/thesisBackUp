package Finance;

/**
* Finance/AccountDetailsHolder.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from Account.idl
* Saturday, April 7, 2012 1:27:11 PM CEST
*/

public final class AccountDetailsHolder implements org.omg.CORBA.portable.Streamable
{
  public Finance.AccountDetails value = null;

  public AccountDetailsHolder ()
  {
  }

  public AccountDetailsHolder (Finance.AccountDetails initialValue)
  {
    value = initialValue;
  }

  public void _read (org.omg.CORBA.portable.InputStream i)
  {
    value = Finance.AccountDetailsHelper.read (i);
  }

  public void _write (org.omg.CORBA.portable.OutputStream o)
  {
    Finance.AccountDetailsHelper.write (o, value);
  }

  public org.omg.CORBA.TypeCode _type ()
  {
    return Finance.AccountDetailsHelper.type ();
  }

}
