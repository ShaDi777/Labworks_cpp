#ifndef LABWORK7_TEST_ID3V2PARSER_H
#define LABWORK7_TEST_ID3V2PARSER_H

#include <memory>
#include <utility>
#include <vector>

enum Encoding {
    ISO_8859_1 = 0, UTF_16 = 1, UTF_16_BE = 2, UTF_8 = 3
};

class Frame {
public:
    struct FrameHeader {
        char frame_id[5];
        int size;
        char flags1;
        char flags2;
    };

    explicit Frame(FrameHeader frame_head) : frame_header(frame_head) {}

    virtual void printInfo() {
        std::cout << "No information\n";
    };

    virtual ~Frame() = default;

    FrameHeader frame_header;
};


class TextFrame : public Frame {
public:
    TextFrame(FrameHeader frame_head, std::vector<char>& parsed_data);

    void printInfo() override;

    ~TextFrame() override = default;

protected:
    Encoding code;
    std::vector<std::string> text;
};

class TextFrameXXX : public Frame {
public:
    TextFrameXXX(FrameHeader frame_head, std::vector<char>& parsed_data);

    void printInfo() override;

    ~TextFrameXXX() override = default;

private:
    Encoding code;
    std::string description;
    std::string value;
};

class UrlFrame : public Frame {
public:
    UrlFrame(FrameHeader frame_head, std::vector<char>& parsed_data);

    void printInfo() override;

    ~UrlFrame() override = default;

protected:
    std::string data;
};

class UrlFrameXXX : public UrlFrame {
public:
    UrlFrameXXX(FrameHeader frame_head, std::vector<char>& parsed_data);

    void printInfo() override;

    ~UrlFrameXXX() override = default;

private:
    Encoding code;
    std::string description;
};

class UFIDFrame : public Frame {
public:
    UFIDFrame(FrameHeader frame_head, std::vector<char>& parsed_data);

    void printInfo() override;

    ~UFIDFrame() override = default;

private:
    std::string owner_id;
    std::string bin_data;
};

class ETCOFrame : public Frame {
public:
    ETCOFrame(FrameHeader frame_head, std::vector<char>& parsed_data);

    void printInfo() override;

    ~ETCOFrame() override = default;

private:
    int time_stamp_format; // 01 - MPEG frames as unit // 02 - milliseconds as units
    std::vector<std::pair<int, int>> typeOfEvent_timeStamp;
};

class USLTFrame : public Frame {
public:
    USLTFrame(FrameHeader frame_head, std::vector<char>& parsed_data);

    void printInfo() override;

    ~USLTFrame() override = default;

private:
    Encoding code;
    char language[4]{};
    std::string content_descriptor;
    std::string text;
};

class SYLTFrame : public Frame {
public:
    SYLTFrame(FrameHeader frame_head, std::vector<char>& parsed_data);

    void printInfo() override;

    ~SYLTFrame() override = default;

private:
    Encoding code;
    char language[4]{};
    int time_stamp_format; // 01 - MPEG frames as unit // 02 - milliseconds as units
    int content_type;
    std::string content_descriptor;
    std::vector<std::pair<std::string, int>> phrase_time; // $xx (xx ..)
};

class COMMFrame : public Frame {
public:
    COMMFrame(FrameHeader frame_head, std::vector<char>& parsed_data);

    void printInfo() override;

    ~COMMFrame() override = default;

private:
    Encoding code;
    char language[4]{};
    std::string short_content_descriptor;
    std::string text;
};

class RVA2Frame : public Frame {
public:
    RVA2Frame(FrameHeader frame_head, std::vector<char>& parsed_data);

    void printInfo() override;

    ~RVA2Frame() override = default;

private:
    std::string id;
    std::vector<std::tuple<int, int, int, int>> channel_volume_peakBits_peak;
};

class EQU2Frame : public Frame {
public:
    EQU2Frame(FrameHeader frame_head, std::vector<char>& parsed_data);

    void printInfo() override;

    ~EQU2Frame() override = default;

private:
    int interpolation_method; // 00 - band , 01 - linear
    std::string id;
    std::vector<std::pair<int, int>> freq_volumeAdj;
};

class PCNTFrame : public Frame {
public:
    PCNTFrame(FrameHeader frame_head, std::vector<char> parsed_data);

    void printInfo() override;

    ~PCNTFrame() override = default;

protected:
    uint64_t count;
};

class POPMFrame : public PCNTFrame {
public:
    POPMFrame(FrameHeader frame_head, std::vector<char>& parsed_data);

    void printInfo() override;

    ~POPMFrame() override = default;

private:
    std::string email;
    int rating;
};

class RBUFFrame : public Frame {
public:
    RBUFFrame(FrameHeader frame_head, std::vector<char>& parsed_data);

    void printInfo() override;

    ~RBUFFrame() override = default;

private:
    int size;
    bool embedded_info_flag;
    int offset;
};

class LINKFrame : public Frame {
public:
    LINKFrame(FrameHeader frame_head, std::vector<char>& parsed_data);

    void printInfo() override;

    ~LINKFrame() override = default;

private:
    char frame_identifier[5]{};
    std::string url;
    std::string data;
};

class POSSFrame : public Frame {
public:
    POSSFrame(FrameHeader frame_head, std::vector<char>& parsed_data);

    void printInfo() override;

    ~POSSFrame() override = default;

private:
    int time_stamp_format;
    uint64_t position;
};

class USERFrame : public Frame {
public:
    USERFrame(FrameHeader frame_head, std::vector<char>& parsed_data);

    void printInfo() override;

    ~USERFrame() override = default;

private:
    Encoding code;
    char language[4]{};
    std::string text;
};

class OWNEFrame : public Frame {
public:
    OWNEFrame(FrameHeader frame_head, std::vector<char>& parsed_data);

    void printInfo() override;

    ~OWNEFrame() override = default;

protected:
    Encoding code;
    std::string price;
    std::string date; // 8 bytes YYYYMMDD
    std::string seller;
};

class COMRFrame : public OWNEFrame {
public:
    COMRFrame(FrameHeader frame_head, std::vector<char>& parsed_data);

    void printInfo() override;

    ~COMRFrame() override = default;

private:
    int received_as;
    std::string seller;
    std::string description;
    std::string picture_mime_type;
    std::string seller_logo;
};

class SEEKFrame : public Frame {
public:
    SEEKFrame(FrameHeader frame_head, std::vector<char>& parsed_data);

    void printInfo() override;

    ~SEEKFrame() override = default;

private:
    int offset;
};

class ENCRFrame : public Frame {
public:
    ENCRFrame(FrameHeader frame_head, std::vector<char>& parsed_data);

    void printInfo() override;

    ~ENCRFrame() override = default;

private:
    std::string owner_id;
    char method_symbol;
    std::string encryption_data;
};

class GRIDFrame : public Frame {
public:
    GRIDFrame(FrameHeader frame_head, std::vector<char>& parsed_data);

    void printInfo() override;

    ~GRIDFrame() override = default;

private:
    std::string owner_id;
    char group_symbol;
    std::string group_data;
};

class PRIVFrame : public Frame {
public:
    PRIVFrame(FrameHeader frame_head, std::vector<char>& parsed_data);

    void printInfo() override;

    ~PRIVFrame() override = default;

private:
    std::string owner_id;
    std::string private_data;
};


class ID3v2 {
public:
    struct TagHeader {
        char file_id[4];
        int major_version;
        int minor_version;

        bool flag_unsync;
        bool flag_ext_header;
        bool flag_experimental;
        bool flag_footer;

        int size;
    };

    explicit ID3v2(TagHeader tag_head) : tag_header(tag_head) {}

    [[nodiscard]] int byte_size() const {
        return tag_header.size;
    }

    void append(std::unique_ptr<Frame> frame) {
        frames.push_back(std::move(frame));
    }

public:
    TagHeader tag_header;

    std::vector<std::unique_ptr<Frame>> frames;
};

ID3v2 ParseMP3(const std::string& file_path);

std::vector<Frame*> GetFrame(const ID3v2& meta_data, const char* frame_id);

#endif //LABWORK7_TEST_ID3V2PARSER_H
