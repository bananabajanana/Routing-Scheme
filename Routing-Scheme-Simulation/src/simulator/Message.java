package simulator;

/**
 * Representing a message being sent in the simulation.
 * Currently, only a simple wrapper of the header since we don't care
 * about the contents of the message for our purposes.
 */
public class Message {
  private MessageHeader header;

  public Message(MessageHeader header) {
    this.header = header;
  }

  public Message() {
  }

  public MessageHeader getHeader() {
    return header;
  }

  public void setHeader(MessageHeader header) {
    this.header = header;
  }
}
