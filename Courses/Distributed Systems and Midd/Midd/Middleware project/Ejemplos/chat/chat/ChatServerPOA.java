package chat;


/**
* chat/ChatServerPOA.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from Chat.idl
* Friday, April 6, 2012 6:43:18 PM CEST
*/

public abstract class ChatServerPOA extends org.omg.PortableServer.Servant
 implements chat.ChatServerOperations, org.omg.CORBA.portable.InvokeHandler
{

  // Constructors

  private static java.util.Hashtable _methods = new java.util.Hashtable ();
  static
  {
    _methods.put ("addClient", new java.lang.Integer (0));
    _methods.put ("fwdMsg", new java.lang.Integer (1));
  }

  public org.omg.CORBA.portable.OutputStream _invoke (String $method,
                                org.omg.CORBA.portable.InputStream in,
                                org.omg.CORBA.portable.ResponseHandler $rh)
  {
    org.omg.CORBA.portable.OutputStream out = null;
    java.lang.Integer __method = (java.lang.Integer)_methods.get ($method);
    if (__method == null)
      throw new org.omg.CORBA.BAD_OPERATION (0, org.omg.CORBA.CompletionStatus.COMPLETED_MAYBE);

    switch (__method.intValue ())
    {
       case 0:  // chat/ChatServer/addClient
       {
         chat.ChatClient client = chat.ChatClientHelper.read (in);
         this.addClient (client);
         out = $rh.createReply();
         break;
       }

       case 1:  // chat/ChatServer/fwdMsg
       {
         String msg = chat.MessageHelper.read (in);
         this.fwdMsg (msg);
         out = $rh.createReply();
         break;
       }

       default:
         throw new org.omg.CORBA.BAD_OPERATION (0, org.omg.CORBA.CompletionStatus.COMPLETED_MAYBE);
    }

    return out;
  } // _invoke

  // Type-specific CORBA::Object operations
  private static String[] __ids = {
    "IDL:chat/ChatServer:1.0"};

  public String[] _all_interfaces (org.omg.PortableServer.POA poa, byte[] objectId)
  {
    return (String[])__ids.clone ();
  }

  public ChatServer _this() 
  {
    return ChatServerHelper.narrow(
    super._this_object());
  }

  public ChatServer _this(org.omg.CORBA.ORB orb) 
  {
    return ChatServerHelper.narrow(
    super._this_object(orb));
  }


} // class ChatServerPOA
