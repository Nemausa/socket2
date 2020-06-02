#ifndef _doyou_io_WebSocketObj_HPP_
#define _doyou_io_WebSocketObj_HPP_

#include<cstdint>

namespace doyou {
	namespace io {

		enum WebSocketOpcode {
			opcode_CONTINUATION = 0x0,
			opcode_TEXT = 0x1,
			opcode_BINARY = 0x2,
			opcode_CLOSE = 0x8,
			opcode_PING = 0x9,
			opcode_PONG = 0xA,
		};

		//WebSocket����֡ͷ��Э���ʽ
		struct WebSocketHeader {
			//Extended payload length 16��64λ
			uint64_t len;
			//�������룬Opcode ��ֵ������Ӧ����ν��������� �����غɣ�data payload��
			//������������ǲ���ʶ�ģ���ô���ն�Ӧ�� �Ͽ����ӣ�fail the connection��
			WebSocketOpcode opcode;
			//���дӿͻ��˴��͵�����˵�����֡�������غɶ����������������
			//Mask Ϊ 1����Я���� 4 �ֽڵ� Masking-key��
			//��� Mask Ϊ 0����û�� Masking-key
			//��ע���غ����ݵĳ���len�������� mask key �ĳ��ȡ�
			uint8_t masking_key[4];
			//���ֵΪtrue����ʾ���� ��Ϣ��message�������һ����Ƭ��fragment��
			//����� 0����ʾ���� ��Ϣ��message�������һ�� ��Ƭ��fragment��
			bool fin;
			//��ʾ�Ƿ�Ҫ�����ݣ���Ϣ�壩�����������
			//�ӿͻ��������˷�������ʱ����Ҫ�����ݽ������������
			//�ӷ������ͻ��˷�������ʱ������Ҫ�����ݽ������������
			//�������˽��յ�������û�н��й�����������������Ҫ�Ͽ����ӡ�
			bool mask;
			//�����غɵĳ���,���ֵΪx
			//x Ϊ 0~126�����ݵĳ���Ϊ x �ֽڡ�
			//x Ϊ 126������ 2 ���ֽڴ���һ�� 16 λ���޷������������޷���������ֵΪ���ݵĳ��ȡ�
			//x Ϊ 127������ 8 ���ֽڴ���һ�� 64 λ���޷������������λΪ 0�������޷���������ֵΪ���ݵĳ��ȡ�
			//���⣬��� payload length ռ���˶���ֽڵĻ���payload length �Ķ����Ʊ����� ������big endian����Ҫ��λ��ǰ����
			uint8_t len0;
			//WebSocket����֡ͷ���ֽڳ���
			uint8_t header_size;
		};
	}
}
#endif // !_doyou_io_WebSocketObj_HPP_
