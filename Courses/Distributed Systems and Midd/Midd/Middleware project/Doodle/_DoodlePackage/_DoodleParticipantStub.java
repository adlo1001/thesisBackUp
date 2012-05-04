package _DoodlePackage;


/**
* _DoodlePackage/_DoodleParticipantStub.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from Doodle.idl
* Saturday, April 14, 2012 4:19:04 PM CEST
*/


//----------------------------------------------
public class _DoodleParticipantStub extends org.omg.CORBA.portable.ObjectImpl implements _DoodlePackage.DoodleParticipant
{

  public void printMsg (String msg)
  {
            org.omg.CORBA.portable.InputStream $in = null;
            try {
                org.omg.CORBA.portable.OutputStream $out = _request ("printMsg", true);
                _DoodlePackage.MessageHelper.write ($out, msg);
                $in = _invoke ($out);
                return;
            } catch (org.omg.CORBA.portable.ApplicationException $ex) {
                $in = $ex.getInputStream ();
                String _id = $ex.getId ();
                throw new org.omg.CORBA.MARSHAL (_id);
            } catch (org.omg.CORBA.portable.RemarshalException $rm) {
                printMsg (msg        );
            } finally {
                _releaseReply ($in);
            }
  } // printMsg

  public void startRunning ()
  {
            org.omg.CORBA.portable.InputStream $in = null;
            try {
                org.omg.CORBA.portable.OutputStream $out = _request ("startRunning", true);
                $in = _invoke ($out);
                return;
            } catch (org.omg.CORBA.portable.ApplicationException $ex) {
                $in = $ex.getInputStream ();
                String _id = $ex.getId ();
                throw new org.omg.CORBA.MARSHAL (_id);
            } catch (org.omg.CORBA.portable.RemarshalException $rm) {
                startRunning (        );
            } finally {
                _releaseReply ($in);
            }
  } // startRunning

  // Type-specific CORBA::Object operations
  private static String[] __ids = {
    "IDL:DoodlePackage/DoodleParticipant:1.0"};

  public String[] _ids ()
  {
    return (String[])__ids.clone ();
  }

  private void readObject (java.io.ObjectInputStream s) throws java.io.IOException
  {
     String str = s.readUTF ();
     String[] args = null;
     java.util.Properties props = null;
     org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init (args, props);
   try {
     org.omg.CORBA.Object obj = orb.string_to_object (str);
     org.omg.CORBA.portable.Delegate delegate = ((org.omg.CORBA.portable.ObjectImpl) obj)._get_delegate ();
     _set_delegate (delegate);
   } finally {
     orb.destroy() ;
   }
  }

  private void writeObject (java.io.ObjectOutputStream s) throws java.io.IOException
  {
     String[] args = null;
     java.util.Properties props = null;
     org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init (args, props);
   try {
     String str = orb.object_to_string (this);
     s.writeUTF (str);
   } finally {
     orb.destroy() ;
   }
  }
} // class _DoodleParticipantStub