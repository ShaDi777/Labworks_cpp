#include <iostream>
#include <fstream>
#include <cstring>

#include "ID3v2Parser.h"

TextFrame::TextFrame(FrameHeader frame_head, std::vector<char>& parsed_data) : Frame(frame_head) {
    code = static_cast<Encoding>(parsed_data[0]);
    auto iter = parsed_data.begin() + 1;
    while (iter <= parsed_data.end()) {
        auto iter_back = std::find(iter + 1, parsed_data.end(), '\0');
        std::string temp = std::string(iter, iter_back);
        iter = iter_back + 1 + ((code == UTF_16 || code == UTF_16_BE) ? 1 : 0);
        text.emplace_back(temp);
    }
}

void TextFrame::printInfo() {
    std::cout << frame_header.frame_id << " code:" << code << " ||| ";
    for (const auto& s: text) {
        std::cout << s << '.';
    }
    std::cout << '\n';
}


TextFrameXXX::TextFrameXXX(FrameHeader frame_head, std::vector<char>& parsed_data) : Frame(frame_head) {
    code = static_cast<Encoding>(parsed_data[0]);
    description = std::string(parsed_data.begin() + 1, std::find(parsed_data.begin() + 1, parsed_data.end(), '\0'));
    value = std::string(std::find(parsed_data.begin() + 1, parsed_data.end(), '\0') + 1 +
                        ((code == UTF_16 || code == UTF_16_BE) ? 1 : 0), parsed_data.end());
}

void TextFrameXXX::printInfo() {
    std::cout << frame_header.frame_id << " code:" << code << " ||| " << description << " / " << value << '\n';
}


UrlFrame::UrlFrame(FrameHeader frame_head, std::vector<char>& parsed_data) : Frame(frame_head) {
    data = std::string(parsed_data.begin(), parsed_data.end());
}

void UrlFrame::printInfo() {
    std::cout << frame_header.frame_id << " ||| " << data << '\n';
}


UrlFrameXXX::UrlFrameXXX(FrameHeader frame_head, std::vector<char>& parsed_data) : UrlFrame(frame_head, parsed_data) {
    code = static_cast<Encoding>(parsed_data[0]);
    description = std::string(parsed_data.begin(), std::find(parsed_data.begin() + 1, parsed_data.end(), '\0'));
    data = std::string(std::find(parsed_data.begin() + 1, parsed_data.end(), '\0') + 1 +
                       ((code == UTF_16 || code == UTF_16_BE) ? 1 : 0), parsed_data.end());
}

void UrlFrameXXX::printInfo() {
    std::cout << frame_header.frame_id << " code:" << code << " ||| " << data << " / " << description << '\n';
}


UFIDFrame::UFIDFrame(FrameHeader frame_head, std::vector<char>& parsed_data) : Frame(frame_head) {
    owner_id = std::string(parsed_data.begin(), std::find(parsed_data.begin(), parsed_data.end(), '\0'));
    bin_data = std::string(std::find(parsed_data.begin(), parsed_data.end(), '\0') + 1, parsed_data.end());
}

void UFIDFrame::printInfo() {
    std::cout << frame_header.frame_id << " ||| " << owner_id << " / " << bin_data << '\n';
}

ETCOFrame::ETCOFrame(FrameHeader frame_head, std::vector<char>& parsed_data) : Frame(frame_head) {
    time_stamp_format = (unsigned char) parsed_data[0];
    for (int i = 1; i < parsed_data.size();) {
        int type_of_event = (unsigned char) parsed_data[i++];
        int time_stamp = (parsed_data[i] << 24) + (parsed_data[i + 1] << 16) + (parsed_data[i + 2] << 8) +
                         (parsed_data[i + 3]);
        i += 4;
        typeOfEvent_timeStamp.emplace_back(std::make_pair(type_of_event, time_stamp));
    }
}

void ETCOFrame::printInfo() {
    std::cout << frame_header.frame_id << " ||| " << "Format:" << time_stamp_format << " // ";
    for (auto p: typeOfEvent_timeStamp) {
        std::cout << p.first << '-' << p.second << " / ";
    }
    std::cout << '\n';
}

USLTFrame::USLTFrame(FrameHeader frame_head, std::vector<char>& parsed_data) : Frame(frame_head) {
    code = (Encoding) parsed_data[0];
    language[0] = parsed_data[1];
    language[1] = parsed_data[2];
    language[2] = parsed_data[3];
    language[3] = '\0';
    content_descriptor = std::string(parsed_data.begin() + 4,
                                     std::find(parsed_data.begin() + 4, parsed_data.end(), '\0'));
    text = std::string(std::find(parsed_data.begin() + 4, parsed_data.end(), '\0') + 1 +
                       ((code == UTF_16 || code == UTF_16_BE) ? 1 : 0), parsed_data.end());
}

void USLTFrame::printInfo() {
    std::cout << frame_header.frame_id << " code:" << code << " ||| "
              << "Lang:" << language << " // " << content_descriptor << '/' << text << '\n';
}


SYLTFrame::SYLTFrame(FrameHeader frame_head, std::vector<char>& parsed_data) : Frame(frame_head) {
    code = (Encoding) parsed_data[0];
    language[0] = parsed_data[1];
    language[1] = parsed_data[2];
    language[2] = parsed_data[3];
    language[3] = '\0';
    time_stamp_format = (unsigned char) parsed_data[4];
    content_type = (unsigned char) parsed_data[5];
    auto iter = std::find(parsed_data.begin() + 6, parsed_data.end(), '\0');
    content_descriptor = std::string(parsed_data.begin() + 6, iter);
    iter += 1 + ((code == UTF_16 || code == UTF_16_BE) ? 1 : 0);
    while (iter <= parsed_data.end()) {
        auto iter_back = std::find(iter, parsed_data.end(), '\0');
        std::string phrase = std::string(iter, iter_back);
        iter = iter_back + 1 + ((code == UTF_16 || code == UTF_16_BE) ? 1 : 0);
        int time = (*iter << 24) + (*(iter + 1) << 16) + (*(iter + 2) << 8) + (*(iter + 3));
        iter += 4;
        phrase_time.emplace_back(std::make_pair(phrase, time));
    }
}

void SYLTFrame::printInfo() {
    std::cout << frame_header.frame_id << " code:" << code << " ||| "
              << "Lang:" << language << " // " << content_descriptor << '/'
              << time_stamp_format << '/' << content_type << '/';
    for (const auto& p: phrase_time) {
        std::cout << p.first << '(' << p.second << ')' << '/';
    }
    std::cout << '\n';
}


COMMFrame::COMMFrame(FrameHeader frame_head, std::vector<char>& parsed_data) : Frame(frame_head) {
    code = (Encoding) parsed_data[0];
    language[0] = parsed_data[1];
    language[1] = parsed_data[2];
    language[2] = parsed_data[3];
    language[3] = '\0';
    short_content_descriptor = std::string(parsed_data.begin() + 4,
                                           std::find(parsed_data.begin() + 4, parsed_data.end(), '\0'));
    text = std::string(std::find(parsed_data.begin() + 4, parsed_data.end(), '\0') + 1 +
                       ((code == UTF_16 || code == UTF_16_BE) ? 1 : 0), parsed_data.end());
}

void COMMFrame::printInfo() {
    std::cout << frame_header.frame_id << " code:" << code << " ||| "
              << "Lang:" << language << " // " << short_content_descriptor << '/' << text << '\n';
}


RVA2Frame::RVA2Frame(FrameHeader frame_head, std::vector<char>& parsed_data) : Frame(frame_head) {
    auto iter = std::find(parsed_data.begin(), parsed_data.end(), '\0');
    id = std::string(parsed_data.begin(), iter);
    iter++;
    while (iter <= parsed_data.end()) {
        int channel_type = (unsigned char) *iter;
        int volume_adj = (*(iter + 1) << 8) + *(iter + 2);
        int peak_bits = (unsigned char) *(iter + 3);
        iter += 4;
        int peak = 0;
        while (peak_bits > 0) {
            peak = (peak << 8) + (*iter);
            iter++;
            peak_bits -= 8;
        }
        channel_volume_peakBits_peak.emplace_back(std::make_tuple(channel_type, volume_adj, peak_bits, peak));
    }
}

void RVA2Frame::printInfo() {
    std::cout << frame_header.frame_id << " ||| " << id << " // ";
    for (auto tup: channel_volume_peakBits_peak) {
        std::cout << std::get<0>(tup) << "/" << std::get<1>(tup) << "/" << std::get<2>(tup) << "/"
                  << std::get<3>(tup) << " // ";
    }
    std::cout << '\n';
}


EQU2Frame::EQU2Frame(FrameHeader frame_head, std::vector<char>& parsed_data) : Frame(frame_head) {
    interpolation_method = (unsigned char) parsed_data[0];
    auto iter = std::find(parsed_data.begin() + 1, parsed_data.end(), '\0');
    id = std::string(parsed_data.begin() + 1, iter);
    iter++;
    while (iter <= parsed_data.end()) {
        int frequency = (((unsigned char) *iter << 8) + ((unsigned char) *(iter + 1))) / 2;
        int volume_adj = (((unsigned char) *(iter + 2) << 8) + ((unsigned char) *(iter + 3))) / 512;
        iter += 4;
        freq_volumeAdj.emplace_back(std::make_pair(frequency, volume_adj));
    }
}

void EQU2Frame::printInfo() {
    std::cout << frame_header.frame_id << " ||| "
              << id << " // " << "Interpolation:" << (interpolation_method == 1 ? "linear" : "band");
    for (auto fv: freq_volumeAdj) {
        std::cout << fv.first << "Hz" << '/' << fv.second << "db" << " // ";
    }
    std::cout << '\n';
}


PCNTFrame::PCNTFrame(FrameHeader frame_head, std::vector<char> parsed_data) : Frame(frame_head) {
    for (auto byte: parsed_data) {
        count = (count << 8) + (unsigned char) byte;
    }
}

void PCNTFrame::printInfo() {
    std::cout << frame_header.frame_id << " ||| " << "Listened:" << count << " times" << '\n';
}


POPMFrame::POPMFrame(FrameHeader frame_head, std::vector<char>& parsed_data) :
        PCNTFrame::PCNTFrame(frame_head,
                             std::vector<char>(std::find(parsed_data.begin(), parsed_data.end(), '\0') + 2,
                                               parsed_data.end())) {
    auto iter = std::find(parsed_data.begin(), parsed_data.end(), '\0');
    email = std::string(parsed_data.begin(), iter);
    iter++;
    rating = (unsigned char) *(iter);
}

void POPMFrame::printInfo() {
    std::cout << frame_header.frame_id << " ||| " << "Rating:" << rating << '/' << count << '\n';
}


RBUFFrame::RBUFFrame(FrameHeader frame_head, std::vector<char>& parsed_data) : Frame(frame_head) {
    size = (parsed_data[0] << 16) + (parsed_data[1] << 8) + parsed_data[2];
    embedded_info_flag = parsed_data[3] & 1;
    offset = (parsed_data[4] << 24) + (parsed_data[5] << 16) + (parsed_data[6] << 8) + parsed_data[7];
}

void RBUFFrame::printInfo() {
    std::cout << frame_header.frame_id << " ||| " << "Size:" << size << " / " << "Flag = " << embedded_info_flag
              << " / " << offset << '\n';
}


LINKFrame::LINKFrame(FrameHeader frame_head, std::vector<char>& parsed_data) : Frame(frame_head) {
    for (int i = 0; i < 4; i++) frame_identifier[i] = parsed_data[i];
    frame_identifier[4] = '\0';
    url = std::string(parsed_data.begin() + 4, std::find(parsed_data.begin() + 4, parsed_data.end(), '\0'));
    data = std::string(std::find(parsed_data.begin() + 4, parsed_data.end(), '\0') + 1, parsed_data.end());
}

void LINKFrame::printInfo() {
    std::cout << frame_header.frame_id << " ||| " << frame_identifier << " / " << url << " / " << data << '\n';
}


POSSFrame::POSSFrame(FrameHeader frame_head, std::vector<char>& parsed_data) : Frame(frame_head) {
    time_stamp_format = (unsigned char) parsed_data[0];
    for (int i = 1; i < parsed_data.size(); i++) {
        position = (position << 8) + (unsigned char) parsed_data[i];
    }
}

void POSSFrame::printInfo() {
    std::cout << frame_header.frame_id << " ||| " << time_stamp_format << " / " << position << '\n';
}


USERFrame::USERFrame(FrameHeader frame_head, std::vector<char>& parsed_data) : Frame(frame_head) {
    code = (Encoding) parsed_data[0];
    language[0] = parsed_data[1];
    language[1] = parsed_data[2];
    language[2] = parsed_data[3];
    language[3] = '\0';
    text = std::string(parsed_data.begin() + 4, parsed_data.end());
}

void USERFrame::printInfo() {
    std::cout << frame_header.frame_id << " code:" << code << " ||| " << language << " / " << text << '\n';
}


OWNEFrame::OWNEFrame(FrameHeader frame_head, std::vector<char>& parsed_data)
        : Frame(frame_head) {
    code = (Encoding) parsed_data[0];
    auto iter = std::find(parsed_data.begin() + 1, parsed_data.end(), '\0');
    price = std::string(parsed_data.begin() + 1, iter);
    date = std::string(iter + 1, iter + 9);
    seller = std::string(iter + 9, std::find(iter + 9, parsed_data.end(), '\0'));
}

void OWNEFrame::printInfo() {
    std::cout << frame_header.frame_id << " code:" << code << " ||| " << price << " / " << date << " / " << seller
              << '\n';
}


COMRFrame::COMRFrame(FrameHeader frame_head, std::vector<char>& parsed_data)
        : OWNEFrame::OWNEFrame(frame_head, parsed_data) {
    auto iter = std::find(parsed_data.begin() + 1, parsed_data.end(), '\0') + 9;
    iter = std::find(iter, parsed_data.end(), '\0') + 1;
    received_as = (unsigned char) *iter;
    auto iter_back = std::find(iter + 1, parsed_data.end(), '\0');
    seller = std::string(iter + 1, iter_back);
    iter = iter_back + 1 + ((code == UTF_16 || code == UTF_16_BE) ? 1 : 0);
    iter_back = std::find(iter, parsed_data.end(), '\0');
    description = std::string(iter, iter_back);
    iter = iter_back + 1 + ((code == UTF_16 || code == UTF_16_BE) ? 1 : 0);
    iter_back = std::find(iter, parsed_data.end(), '\0');
    picture_mime_type = std::string(iter, iter_back);
    seller_logo = std::string(iter_back + 1, parsed_data.end());
}

void COMRFrame::printInfo() {
    std::cout << frame_header.frame_id << " code:" << code << " ||| " << price << " / " << date
              << " / " << seller << " / " << seller << " / " << received_as << " / " << seller << " / "
              << description << " / " << picture_mime_type << " / " << seller_logo << '\n';
}


SEEKFrame::SEEKFrame(FrameHeader frame_head, std::vector<char>& parsed_data) : Frame(frame_head) {
    offset = (parsed_data[0] << 24) + (parsed_data[1] << 16) + (parsed_data[2] << 8) + (parsed_data[3]);
}

void SEEKFrame::printInfo() {
    std::cout << frame_header.frame_id << " ||| " << offset << '\n';
}


ENCRFrame::ENCRFrame(FrameHeader frame_head, std::vector<char>& parsed_data) : Frame(frame_head) {
    auto iter = std::find(parsed_data.begin(), parsed_data.end(), '\0');
    owner_id = std::string(parsed_data.begin(), iter);
    method_symbol = *(iter + 1);
    encryption_data = std::string(iter + 2, parsed_data.end());
}

void ENCRFrame::printInfo() {
    std::cout << frame_header.frame_id << " ||| " << owner_id << " / " << method_symbol << " / " << encryption_data
              << '\n';
}


GRIDFrame::GRIDFrame(FrameHeader frame_head, std::vector<char>& parsed_data) : Frame(frame_head) {
    auto iter = std::find(parsed_data.begin(), parsed_data.end(), '\0');
    owner_id = std::string(parsed_data.begin(), iter);
    group_symbol = *(iter + 1);
    group_data = std::string(iter + 2, parsed_data.end());
}

void GRIDFrame::printInfo() {
    std::cout << frame_header.frame_id << " ||| " << owner_id << " / " << group_symbol << " / " << group_data
              << '\n';
}


PRIVFrame::PRIVFrame(FrameHeader frame_head, std::vector<char>& parsed_data) : Frame(frame_head) {
    auto iter = std::find(parsed_data.begin(), parsed_data.end(), '\0');
    owner_id = std::string(parsed_data.begin(), iter);
    private_data = std::string(iter + 1, parsed_data.end());
}

void PRIVFrame::printInfo() {
    std::cout << frame_header.frame_id << " ||| " << owner_id << " / " << private_data << '\n';
}


int synchsafe_size(const char* data) {
    return (data[0] << 3 * 7) | (data[1] << 2 * 7) | (data[2] << 1 * 7) | (data[3]);
}

ID3v2::TagHeader ReadTagHeader(std::ifstream& file) {
    ID3v2::TagHeader tag_header{};
    char head[10];
    file.read(head, 10);

    tag_header.file_id[0] = head[0];
    tag_header.file_id[1] = head[1];
    tag_header.file_id[2] = head[2];
    tag_header.file_id[3] = '\0';

    tag_header.major_version = (unsigned char) head[3];
    tag_header.minor_version = (unsigned char) head[4];

    tag_header.flag_unsync = (head[5] >> 7) & 1;
    tag_header.flag_ext_header = (head[5] >> 6) & 1;
    tag_header.flag_experimental = (head[5] >> 5) & 1;
    tag_header.flag_footer = (head[5] >> 4) & 1;

    tag_header.size = synchsafe_size(&head[6]);

    if (tag_header.flag_ext_header) file.read(head, 6);

    return tag_header;
}

std::unique_ptr<Frame> ReadFrame(std::ifstream& file, int& iter) {
    Frame::FrameHeader frame_header{};

    char head[10];
    file.read(head, 10);

    if ((head[0] || head[1] || head[2] || head[3] ||
         head[4] || head[5] || head[6] || head[7] ||
         head[8] || head[9]) == 0) {
        iter += 10;
        return nullptr;
    }

    frame_header.frame_id[0] = head[0];
    frame_header.frame_id[1] = head[1];
    frame_header.frame_id[2] = head[2];
    frame_header.frame_id[3] = head[3];
    frame_header.frame_id[4] = '\0';

    frame_header.size = synchsafe_size(&head[4]);

    frame_header.flags1 = head[8];
    frame_header.flags2 = head[9];

    iter += (10 + frame_header.size);


    std::vector<char> frame_data(frame_header.size);

    file.read(reinterpret_cast<char*>(frame_data.data()), frame_header.size);

    std::unique_ptr<Frame> frame_ptr = nullptr;

    if (frame_header.frame_id[0] == 'T') {
        if (frame_header.frame_id[1] == 'X' && frame_header.frame_id[2] == 'X' && frame_header.frame_id[3] == 'X') {
            frame_ptr = std::make_unique<TextFrameXXX>(TextFrameXXX(frame_header, frame_data));
        } else {
            frame_ptr = std::make_unique<TextFrame>(TextFrame(frame_header, frame_data));
        }
    } else if (frame_header.frame_id[0] == 'W') {
        if (frame_header.frame_id[1] == 'X' && frame_header.frame_id[2] == 'X' && frame_header.frame_id[3] == 'X') {
            frame_ptr = std::make_unique<UrlFrameXXX>(UrlFrameXXX(frame_header, frame_data));
        } else {
            frame_ptr = std::make_unique<UrlFrame>(UrlFrame(frame_header, frame_data));
        }
    } else if (strcmp(frame_header.frame_id, "UFID") == 0) {
        frame_ptr = std::make_unique<UFIDFrame>(UFIDFrame(frame_header, frame_data));

    } else if (strcmp(frame_header.frame_id, "ETCO") == 0) {
        frame_ptr = std::make_unique<ETCOFrame>(ETCOFrame(frame_header, frame_data));

    } else if (strcmp(frame_header.frame_id, "USLT") == 0) {
        frame_ptr = std::make_unique<USLTFrame>(USLTFrame(frame_header, frame_data));

    } else if (strcmp(frame_header.frame_id, "SYLT") == 0) {
        frame_ptr = std::make_unique<SYLTFrame>(SYLTFrame(frame_header, frame_data));

    } else if (strcmp(frame_header.frame_id, "COMM") == 0) {
        frame_ptr = std::make_unique<COMMFrame>(COMMFrame(frame_header, frame_data));

    } else if (strcmp(frame_header.frame_id, "RVA2") == 0) {
        frame_ptr = std::make_unique<RVA2Frame>(RVA2Frame(frame_header, frame_data));

    } else if (strcmp(frame_header.frame_id, "EQU2") == 0) {
        frame_ptr = std::make_unique<EQU2Frame>(EQU2Frame(frame_header, frame_data));

    } else if (strcmp(frame_header.frame_id, "PCNT") == 0) {
        frame_ptr = std::make_unique<PCNTFrame>(PCNTFrame(frame_header, frame_data));

    } else if (strcmp(frame_header.frame_id, "POPM") == 0) {
        frame_ptr = std::make_unique<POPMFrame>(POPMFrame(frame_header, frame_data));

    } else if (strcmp(frame_header.frame_id, "RBUF") == 0) {
        frame_ptr = std::make_unique<RBUFFrame>(RBUFFrame(frame_header, frame_data));

    } else if (strcmp(frame_header.frame_id, "LINK") == 0) {
        frame_ptr = std::make_unique<LINKFrame>(LINKFrame(frame_header, frame_data));

    } else if (strcmp(frame_header.frame_id, "POSS") == 0) {
        frame_ptr = std::make_unique<POSSFrame>(POSSFrame(frame_header, frame_data));

    } else if (strcmp(frame_header.frame_id, "USER") == 0) {
        frame_ptr = std::make_unique<USERFrame>(USERFrame(frame_header, frame_data));

    } else if (strcmp(frame_header.frame_id, "OWNE") == 0) {
        frame_ptr = std::make_unique<OWNEFrame>(OWNEFrame(frame_header, frame_data));

    } else if (strcmp(frame_header.frame_id, "COMR") == 0) {
        frame_ptr = std::make_unique<COMRFrame>(COMRFrame(frame_header, frame_data));

    } else if (strcmp(frame_header.frame_id, "SEEK") == 0) {
        frame_ptr = std::make_unique<SEEKFrame>(SEEKFrame(frame_header, frame_data));

    } else if (strcmp(frame_header.frame_id, "ENCR") == 0) {
        frame_ptr = std::make_unique<ENCRFrame>(ENCRFrame(frame_header, frame_data));

    } else if (strcmp(frame_header.frame_id, "GRID") == 0) {
        frame_ptr = std::make_unique<GRIDFrame>(GRIDFrame(frame_header, frame_data));

    } else if (strcmp(frame_header.frame_id, "PRIV") == 0) {
        frame_ptr = std::make_unique<PRIVFrame>(PRIVFrame(frame_header, frame_data));
    }

    return frame_ptr;
}

ID3v2 ParseMP3(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);

    ID3v2 data = ID3v2(ReadTagHeader(file));

    for (int i = 0; i < data.byte_size() - 10;) {
        std::unique_ptr<Frame> frame = ReadFrame(file, i);

        if (frame != nullptr) {
            data.append(std::move(frame));
        }
    }

    return data;
}

std::vector<Frame*> GetFrame(const ID3v2& meta_data, const char* frame_id) {
    std::vector<Frame*> found_frames;
    for (auto& f: meta_data.frames) {
        if (strcmp(f->frame_header.frame_id, frame_id) == 0) {
            found_frames.emplace_back(f.get());
        }
    }
    return found_frames;
}
