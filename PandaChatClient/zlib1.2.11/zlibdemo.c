//zlibѹ�����ѹʹ��ʾ��
//#include "zlib.h"

//int CompressBuf()
//{
//    char text[] = "zlib compress and uncompress test\nturingo@163.com\n2012-11-05\n";
//    uLong nSrcLen = strlen(text) + 1;  /* ��Ҫ���ַ����Ľ�����'\0'Ҳһ������ */
//    char* buf = NULL;
//    uLong nDestLen;
//
//    /* ���㻺������С����Ϊ������ڴ� */
//    nDestLen = compressBound(nSrcLen); /* ѹ����ĳ����ǲ��ᳬ��nDestLen�� */
//    if ((buf = (char*)malloc(sizeof(char) * nDestLen)) == NULL)
//    {
//        printf("no enough memory!\n");
//        return -1;
//    }
//
//    /* ѹ�� */
//    if (compress((Bytef*)buf, &nDestLen, (const Bytef*)text, nSrcLen) != Z_OK)
//    {
//        printf("compress failed!\n");
//        return -1;
//    }
//
//    /* ��ѹ�� */
//    if (uncompress((Bytef*)text, &nSrcLen, (const Bytef*)buf, nDestLen) != Z_OK)
//    {
//        printf("uncompress failed!\n");
//        return -1;
//    }
//
//    /* ��ӡ��������ͷ��ڴ� */
//    printf("%s", text);
//    if (buf != NULL)
//    {
//        free(buf);
//        buf = NULL;
//    }
//
//    return 0;
//}