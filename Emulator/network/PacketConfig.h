#pragma once

#include "PacketData.h"

#pragma warning(disable: 4311) 
#pragma warning(disable: 4302) 
using YCM_PACKET = CHROZEN_PACKET;

class PacketConfig
{
public:
    static YCM_PACKET* GetPacket(byte* buffer)
    {
        return reinterpret_cast<YCM_PACKET*>(buffer);
    }

    static YCM_PACKET* GetPacket(byte* buffer, int packet_code)
    {
        YCM_PACKET* packet = GetPacket(buffer);
        packet->nPacketCode = packet_code;

        return packet;
    }

    static bool IsIncludePacketCode(byte* buffer)
    {
        unsigned int packet_code = GetPacketCode(buffer);

        bool ret = false;
        switch (packet_code)
        {
        case PACKCODE_SYSTEM_INFORM:
        case PACKCODE_SYSTEM_CONFIG:
        case PACKCODE_RUN_SETUP:
        case PACKCODE_TIME_EVENT:
        case PACKCODE_MIX_SETUP:
        case PACKCODE_SEQUENCE:
        case PACKCODE_COMMAND:
        case PACKCODE_STATE:
        case PACKCODE_SLFEMSG:
        case PACKCODE_SERVICE:
        case PACKCODE_ADJUST_DATA:
        case PACKCODE_DIAG_DATA:
        case PACKCODE_USED_TIME:
        case PACKCODE_UPDATE:
        case PACKCODE_SPECIAL:
            ret = true;
            break;
        default:
            break;
        }

        return ret;
    }

    static unsigned int GetDeviceId(byte* buffer)
    {
        unsigned int pos = sizeof(YCM_PACKET::nPacketLength);
        unsigned int device_id = 0;

        memcpy(&device_id, buffer + pos, sizeof(device_id));

        return device_id;
    }

    static unsigned int GetPacketCode(byte* buffer)
    {
        unsigned int pos = sizeof(YCM_PACKET::nPacketLength) + sizeof(YCM_PACKET::nDeviceID);
        unsigned int packet_code = 0;

        memcpy(&packet_code, buffer + pos, sizeof(packet_code));

        return packet_code;
    }

    static unsigned int GetSlotSize(byte* buffer)
    {
        unsigned int pos =
            sizeof(YCM_PACKET::nPacketLength) +
            sizeof(YCM_PACKET::nDeviceID) +
            sizeof(YCM_PACKET::nPacketCode) +
            sizeof(YCM_PACKET::nEventIndex) +
            sizeof(YCM_PACKET::nSlotOffset);

        unsigned int slot_size = 0;

        memcpy(&slot_size, buffer + pos, sizeof(slot_size));

        return slot_size;
    }

    static unsigned int GetPacketLength(byte* buffer)
    {
        //int* packet_len = (int*)buffer;
        //return *packet_len;
        unsigned int packet_len = 0;
        memcpy(&packet_len, buffer, sizeof(packet_len));
        return packet_len;
    }

    static unsigned int GetPacketLength(YCM_PACKET* packet)
    {
        return packet->nPacketLength;
    }

    // buffer     : 조립한 packet이 저장될 pointer
    // full_data  : packet code에 해당하는 data 변수
    // entry_data : full_data 변수 자체 혹은 그 멤버 변수
    // request    : 명령 packet이면 0, 응답 요구 packet이면 1
    // (Request의 경우 packet 1 / Response의 경우 0
    template<typename full_struct, typename entry_struct>
    static bool MakePacket(byte* buffer, full_struct& full_data, entry_struct& entry_data, int request)
    {
        YCM_PACKET* packet = GetPacket(buffer);

        return MakePacket(packet, full_data, entry_data, request);
    }

    template<typename full_struct, typename entry_struct>
    static bool MakePacket(YCM_PACKET* packet, full_struct& full_data, entry_struct& entry_data, int request)
    {
        packet->nPacketLength = sizeof(YCM_PACKET);
        packet->nSlotOffset   = reinterpret_cast<int>(&entry_data) - reinterpret_cast<int>(&full_data);
        packet->nSlotSize     = sizeof(entry_data);

        if (0 == request)
        {
            packet->nPacketLength += sizeof(entry_data);

            memcpy(packet->Slot, &entry_data, sizeof(entry_data));
            // packet->Slot 의 포인터 위치는 buffer + sizeof(YCM_PACKET) 와 같다.
            //memcpy(buffer + sizeof(YCM_PACKET), &entry_data, sizeof(entry_data));
        }

        return true;
    }

    // 데이터를 요구하는 Packet인지 확인 
    static bool IsRequestPacket(YCM_PACKET* packet)
    {
        int slot_size = (0 == packet->nSlotSize) ? 1 : packet->nSlotSize;
        bool ret = packet->nPacketLength < sizeof(YCM_PACKET) + slot_size;

        return ret;
    }

    // 데이터를 요구하는 Packet인지 확인 
    static bool IsRequestPacket(byte* buffer)
    {
        YCM_PACKET* packet = GetPacket(buffer);
        return IsRequestPacket(packet);
    }

    // PACKET이 적정한가 확인
    template<typename full_struct>
    static bool IsValidPacket(byte* buffer, const full_struct& full_data)
    {
        return IsValidPacket(GetPacket(buffer), full_data);
    }

    template<typename full_struct>
    static bool IsValidPacket(YCM_PACKET* packet, const full_struct& full_data)
    {
        //bool ret = packet->nSlotOffset + packet->nSlotSize <= sizeof(full_data);

        bool ret = false;
        int full_data_size = sizeof(full_data);
        int check_size = packet->nSlotOffset + packet->nSlotSize;

        if (full_data_size == check_size)
            ret = true;

        return ret;
    }

    // buffer    : 받은 packet, 응답 요구 packet이면 응답 packet으로 재조립됨
    // full_data : packet_code에 해당하는 data 변수
    // response  : 응답 packet으로 재조립되면 1로 셋팅됨
    template<typename full_struct>
    static bool ApplyPacket(YCM_PACKET* packet, full_struct& full_data, int& response)
    {
        if (IsRequestPacket(packet))
        {
            packet->nPacketLength = sizeof(YCM_PACKET) + packet->nSlotSize;
            memcpy(packet->Slot, &full_data + packet->nSlotOffset, packet->nSlotSize);
            response = 1;
        }
        else
        {
            memcpy(&full_data + packet->nSlotOffset, packet->Slot, packet->nSlotSize);
            response = 0;
        }

        return true;
    }

    template<typename full_struct>
    static bool ApplyPacket(byte* buffer, full_struct& full_data, int& response)
    {
        YCM_PACKET* packet = GetPacket(buffer);

        return ApplyPacket(packet, full_data, response);
    }

    static byte* FindPacket(byte* buffer, int len, int device_id = 0)
    {
        byte* ret_buffer = nullptr;

        // Packet 의 유효한지 확인 한다.
        // Packet 의 유효성은 Driver 에서 사용중인 Packet code 와 Device ID 를 통하여 확인한다.
        // [주의사항] 
        //  - Driver 마다 사용하는 Packet code 가 틀리기 때문에 확인 필요
        //  - Driver 마다 사용하는 Device ID 가 0 값을 가지는지 확인 필요
        for (int ii = 0 ; ii < len - 8 ; ii++)
        {
            byte* packet_temp = buffer + ii;
            if (PacketConfig::IsIncludePacketCode(packet_temp)) // 유효한 Packet code 를 사용중인지 확인
            {
                int packet_device_id = PacketConfig::GetDeviceId(packet_temp);
                if (-1 == device_id) // Device ID 체크 안함
                {
                    ret_buffer = packet_temp;
                    break;
                }
                else if (device_id == packet_device_id || 0 == packet_device_id) // Device ID 가 유효한지 확인
                {
                    ret_buffer = packet_temp;
                    break;
                }

                LOGW << fmt::format("Can't find frame  len[{}] DeviceId[{:#010x}]", len, PacketConfig::GetDeviceId(packet_temp));
            }
        }

        return ret_buffer;
    }

};
#pragma warning(default: 4311)
#pragma warning(default: 4302) 
