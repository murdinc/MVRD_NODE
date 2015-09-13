// PACKET STRUCT
struct PacketObj {
    int16_t leftX, leftY, rightX, rightY; // - - + // Maybe convert to uint_8?
  
    // Triggers
    uint8_t l2; // 0-255
    uint8_t r2; // 0-255
    
    // Buttons - toggle
    bool a = 0;
    bool b = 0;
    bool x = 0;
    bool y = 0;
  
    // Buttons - momentary
    bool l1 = 0;
    bool r1 = 0;
    bool l3 = 0;
    bool r3 = 0;
  
    // D Pad - momentary
    bool d_up;
    bool d_down;
    bool d_left;
    bool d_right;
};

class Node {
    public:
    void readPacket();
  
    PacketObj packet;
    bool success = false;
  
    private:
    //int timeout = 2500;  // default value of NT (if NT command fails)
    //AtCommandRequest at_request = AtCommandRequest();
    //AtCommandResponse at_response = AtCommandResponse();
    
    Rx16Response rx = Rx16Response();
  
};

Node node;


