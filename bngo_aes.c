#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/aes.h>
#include <time.h>

//char PAYLOAD[1024]  =  "SZH1-BINGOV1-ac83f3d9c89c";

/*
 * key:密钥  encryptSrc：原始数据 encryptDst:加密数据
 */
int mqtt_ecb_encrypt(char *key,const char *encryptSrc,unsigned char *encryptDst);
/*
 * key:密钥  decryptSrc：加密数据 decryptDst:解码数据
 */
int mqtt_ecb_decrypt(char *key,const unsigned char *decryptSrc,unsigned char *decryptDst);

int bngoID_AES(const char *bngoID,char *bngoID_aes){

  unsigned char msg[100] = {0};  
  //unsigned char result[100] = {0};
  
  //char msg_base64_encode[100] = {0};
  //unsigned char msg_base64_decode[100] = {0};
  
  char userkey[50] = "2017irelszhy2018";

  mqtt_ecb_encrypt(userkey,bngoID,msg);
  
  //base64_encode( msg, msg_base64_encode, strlen((char *)msg));
  //printf("size:%d  base64_encode:%s \n",strlen(msg_base64_encode),msg_base64_encode);
  
  memcpy(bngoID_aes,msg,100);
  
  
  
  //base64_decode(msg_base64_encode,msg_base64_decode);
   
  //printf("size:%d  msg_base64_decode:%s \n",strlen((char *)msg_base64_encode),msg_base64_decode);

  //mqtt_ecb_decrypt(userkey,msg_base64_decode,result);
   
  return 0;
}

int mqtt_ecb_encrypt(char *key,const char *encryptSrc,unsigned char *encryptDst)
{
	unsigned char *data = (unsigned char *)encryptSrc;
	//printf("原始数据：%s\n",data);	
	//int length = ((strlen((char *)data)+AES_BLOCK_SIZE-1)/AES_BLOCK_SIZE)*AES_BLOCK_SIZE;  //对齐分组	
	int length = ((strlen((char *)data)+AES_BLOCK_SIZE)/AES_BLOCK_SIZE)*AES_BLOCK_SIZE;  //对齐分组
	
	unsigned char userkey[AES_BLOCK_SIZE];
	unsigned char *encrypt_result = malloc(length);
	AES_KEY en_key;
	
	memset((void*)userkey,0,AES_BLOCK_SIZE);
	memset((void*)encrypt_result, 0, length);
	memcpy(userkey,key,AES_BLOCK_SIZE);
	
	AES_set_encrypt_key(userkey, AES_BLOCK_SIZE*8, &en_key);
	
	int len = 0;
	while(len < length) {
		AES_encrypt(data+len, encrypt_result+len, &en_key);
		len += AES_BLOCK_SIZE;
	}
	
	memcpy(encryptDst,encrypt_result,length);
	
	free(encrypt_result);
	
	int i = 0;
	//字节流转换为十六进制字符串
	char szTmp[3]; 
	char byteSrc[100] = {0};
	
	//printf("length:%d 加密后的数据：\n",length);
	for(i = 0;i<length;i++){
		//printf("%02x",encryptDst[i]);
		sprintf( szTmp, "%02X", encryptDst[i] ); 
		memcpy( &byteSrc[i * 2], szTmp, 2 );  
	}	
	//putchar('\n');
	
	memcpy(encryptDst,byteSrc,100);

	return 0;
}
int mqtt_ecb_decrypt(char *key,const unsigned char *decryptSrc,unsigned char *decryptDst)
{
	unsigned char *data = (unsigned char *)decryptSrc;
	
	int length = ((strlen((char *)data)+AES_BLOCK_SIZE)/AES_BLOCK_SIZE)*AES_BLOCK_SIZE;  //对齐分组
	
	unsigned char userkey[AES_BLOCK_SIZE];
	
	AES_KEY de_key;
	
	unsigned char *decrypt_result = malloc(length);

	memset((void*)userkey,0,AES_BLOCK_SIZE);
	memset((void*)decrypt_result, 0, length);
	memcpy(userkey,key,AES_BLOCK_SIZE);

	AES_set_decrypt_key(userkey, AES_BLOCK_SIZE*8, &de_key);

	int len = 0;
	/*循环解密*/
	while(len < length) {
		AES_decrypt(data+len, decrypt_result+len, &de_key);
		len += AES_BLOCK_SIZE;
		}
	printf("解密结果：%s\n",decrypt_result);	
	
	memcpy(decryptDst,decrypt_result,strlen((char *)decrypt_result)+1);
	free(decrypt_result);
	return 0;
}