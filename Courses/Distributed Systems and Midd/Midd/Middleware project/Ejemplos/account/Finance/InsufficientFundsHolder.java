package Finance;

/**
* Finance/InsufficientFundsHolder.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from Account.idl
* Saturday, April 7, 2012 1:27:11 PM CEST
*/

public final class InsufficientFundsHolder implements org.omg.CORBA.portable.Streamable
{
  public Finance.InsufficientFunds value = null;

  public InsufficientFundsHolder ()
  {
  }

  public InsufficientFundsHolder (Finance.InsufficientFunds initialValue)
  {
    value = initialValue;
  }

  public void _read (org.omg.CORBA.portable.InputStream i)
  {
    value = Finance.InsufficientFundsHelper.read (i);
  }

  public void _write (org.omg.CORBA.portable.OutputStream o)
  {
    Finance.InsufficientFundsHelper.write (o, value);
  }

  public org.omg.CORBA.TypeCode _type ()
  {
    return Finance.InsufficientFundsHelper.type ();
  }

}