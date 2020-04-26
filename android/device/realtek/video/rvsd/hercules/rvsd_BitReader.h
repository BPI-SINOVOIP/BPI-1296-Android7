
class BitReader
{
public:
    BitReader(unsigned char *p_input, int size);
    ~BitReader();

    int GetBit(int amount);
    unsigned int GetUE();
    int GetSE();

    int GetInputSize() const;

private:
    bool ReadInput(unsigned char &data);
    void WriteBuffer();

    unsigned int m_buffer;
    int m_bit_hold;

    unsigned char *mp_input_buffer;
    int m_input_size;
    int m_input_read;

    int m_zero_count;
};

