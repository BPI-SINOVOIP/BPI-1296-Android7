#include "rtk_ini.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STR_MARK "'"
#define STR_MARK_NUM 39  // 39: '
#define STR_EQUL "="
#define STR_NULL ""

#define LINE_BUF_SIZE 256
#define FREE(p) {if(p){free((void *)p); p=NULL;}}
#define DELETE(p) {if(p){delete p; p=NULL;}}
#define SAFE_ATOI(x) ((x)?(atoi(x)):0)
#define SAFE_ATOLL(x) ((x)?(atoll(x)):0)
//#define jsprintf(format,arg...)  printf( format,## arg)
#define jsprintf(format,arg...)  

/*
class LLKeyValue start
*/
Rtk_Ini::LLKeyValue::LLKeyValue(char *key,char *value)
{
    if(key)
        m_pKey=strdup(key);
    else
        m_pKey=NULL;
    if(value)
        m_pValue=strdup(value);
    else
        m_pValue=NULL;
    m_NextKeyValue=NULL;
}
Rtk_Ini::LLKeyValue::~LLKeyValue()
{
    DELETE(m_NextKeyValue);
    FREE(m_pKey);
    FREE(m_pValue);
}
int Rtk_Ini::LLKeyValue::getKeyValue(char **key,char **value)
{
    *key=m_pKey;
    *value=m_pValue;
    return 0;
}
int Rtk_Ini::LLKeyValue::setKeyValue(char *key,char *value)
{
    FREE(m_pKey);
    FREE(m_pValue);
    if(key)
        m_pKey=strdup(key);
    else
        m_pKey=NULL;
    if(value)
        m_pValue=strdup(value);
    else
        m_pValue=NULL;
    return 0;
}
Rtk_Ini::LLKeyValue *Rtk_Ini::LLKeyValue::getNextKeyValue()
{
    return m_NextKeyValue;
}
int Rtk_Ini::LLKeyValue::setNextKeyValue(char *key,char *value)
{
    int ret=0;
    if(m_NextKeyValue)
        ret = -1;
    else
        m_NextKeyValue=new LLKeyValue(key,value);
    return ret;
}
/*
class LLSection start
*/
Rtk_Ini::LLSection::LLSection(char *section)
{
    if(section)
        m_pSection=strdup(section);
    m_Next_Section=NULL;
    m_HeadKeyValue=NULL;
}
Rtk_Ini::LLSection::~LLSection()
{
    DELETE(m_Next_Section);
    DELETE(m_HeadKeyValue);
    FREE(m_pSection);
}
int Rtk_Ini::LLSection::getSectionName(char **section)
{
    *section=m_pSection;
    return 0;
}
int Rtk_Ini::LLSection::setSectionName(char *section)
{
    FREE(m_pSection);
    if(section)
        m_pSection=strdup(section);
    else
        m_pSection=NULL;
    return 0;
}
Rtk_Ini::LLKeyValue *Rtk_Ini::LLSection::getHeadKeyValue()
{
    return m_HeadKeyValue;
}
int Rtk_Ini::LLSection::setHeadKeyValue(char *key,char *value)
{
    int ret=0;
    if(m_HeadKeyValue)
        ret=m_HeadKeyValue->setKeyValue(key,value);
    else
    {
        m_HeadKeyValue=new LLKeyValue(key,value);
    }
    return ret;
}
Rtk_Ini::LLSection *Rtk_Ini::LLSection::getNextSection()
{
    return m_Next_Section;
}
int Rtk_Ini::LLSection::setNextSection(char *section)
{
    int ret=0;
    if(m_Next_Section)
        ret = -1;
    else
        m_Next_Section=new LLSection(section);
    return ret;
}

/*
Main Program Start
*/

Rtk_Ini::Rtk_Ini()
{
    m_SectionHeader=NULL;
    m_SectionBuffer=NULL;
}
Rtk_Ini::~Rtk_Ini()
{
    if(m_SectionHeader)
        DELETE(m_SectionHeader);
}

int Rtk_Ini::loadFile(char *filename)
{
    FILE *pIniFile;
    char line[LINE_BUF_SIZE];
    LLSection *pLLSection=NULL;
    if((pIniFile= fopen(filename,"r"))==NULL)
    {
        return -1;
    }
    if(m_SectionHeader)
        DELETE(m_SectionHeader);
    while(fgets(line,LINE_BUF_SIZE-1,pIniFile)!=NULL)
    {
        /*
        1. check section patterm
            Y : append Section and keep section pointer
            N : do step 2
        2. check key value  patterm
            Y : append Key value by section section pointer
            N : continue
        */
        char *section,*key,*value;
        if(!getSection_needfree(line,&section))
        {
            pLLSection=appendSection(section);
            FREE(section)
        }
        else if(!getKeyValue_needfree(line,&key,&value))
        {
            jsprintf("key = %s , value =%s \n",key,value);
            if(pLLSection)
                appendKeyValue(pLLSection,key,value);
            FREE(key);
            FREE(value);
        }
    }
    fclose(pIniFile);
    return 0;
}
int Rtk_Ini::saveFile(char *filename)
{
    FILE *pOutFile;
    if((pOutFile= fopen(filename,"w"))==NULL)
    {
        return -1;
    }
    if(m_SectionHeader)
    {
        LLSection *TempLLSection;
        TempLLSection=m_SectionHeader;
        while(1)
        {
            if(TempLLSection)
            {
                char *SectionName;
                if(!TempLLSection->getSectionName(&SectionName))
                    if(SectionName)
                        fprintf(pOutFile,"[%s]\n",SectionName);
                    else
                        fprintf(pOutFile,"[]\n");
                LLKeyValue *TempLLKeyValue;
                if((TempLLKeyValue=TempLLSection->getHeadKeyValue())!=NULL)
                {
                    while(1)
                    {
                        if(TempLLKeyValue)
                        {
                            char *pInKey,*pInValue;
                            TempLLKeyValue->getKeyValue(&pInKey,&pInValue);
                            if(pInKey && pInValue)
                                fprintf(pOutFile,"%s=%s\n",pInKey,pInValue);
                            TempLLKeyValue=TempLLKeyValue->getNextKeyValue();
                        }
                        else
                            break;
                    }
                }
                TempLLSection=TempLLSection->getNextSection();
            }
            else
                break;
        }
    }
    fclose(pOutFile);
    return 0;
}
char *Rtk_Ini::getStrValue(char *section,char *key)
{
    char *ret=NULL;
    LLKeyValue *TempKeyValue;
    if((TempKeyValue=findKeyValue(section,key))!=NULL)
    {
        char *pInKey,*pInValue;
        TempKeyValue->getKeyValue(&pInKey,&pInValue);
        ret=pInValue;
    }
    return ret;
}
int Rtk_Ini::setStrValue(char *section,char *key,char *value)
{
    int ret=0;
    LLSection *TempLLSection;
    if((TempLLSection=findSection(section)) != NULL)
    {
        LLKeyValue *TempKeyValue;
        if((TempKeyValue=findKeyValue(TempLLSection,key)) != NULL)
        {
            TempKeyValue->setKeyValue(key,value);
        }
        else // append key value
        {
            if(!appendKeyValue(TempLLSection,key,value))
                ret=-1;
        }
    }
    else //append section and key value
    {
        if((TempLLSection=appendSection(section))!=NULL)
        {
            if(!appendKeyValue(TempLLSection,key,value))
                ret=-1;
        }
        else
            ret=-1;
    }
    return ret;
}
int Rtk_Ini::getIntValue(char *section,char *key,int *value)
{
    int ret=-1;
    LLKeyValue *TempKeyValue;
    if((TempKeyValue=findKeyValue(section,key))!=NULL)
    {
        char *pInKey,*pInValue;
        TempKeyValue->getKeyValue(&pInKey,&pInValue);
        *value=SAFE_ATOI(pInValue);
        ret=0;
    }
    return ret;
}
int Rtk_Ini::setIntValue(char *section,char *key,int value)
{
    char TempValue[64];
    sprintf(TempValue,"%d",value);
    return setStrValue(section,key,TempValue);
}
int Rtk_Ini::getU64IntValue(char *section,char *key,u_int64_t *value)
{
    int ret=-1;
    LLKeyValue *TempKeyValue;
    if((TempKeyValue=findKeyValue(section,key))!=NULL)
    {
        char *pInKey,*pInValue;
        TempKeyValue->getKeyValue(&pInKey,&pInValue);
        *value=SAFE_ATOLL(pInValue);
        ret=0;
    }
    return ret;
}
int Rtk_Ini::setU64IntValue(char *section,char *key,u_int64_t value)
{
    char TempValue[64];
    sprintf(TempValue,"%lld",value);
    return setStrValue(section,key,TempValue);
}
int Rtk_Ini::addStrMark_needfree(char *source,char **output)
{
    int ret=-1;
    if(source)
    {
        *output=(char *)malloc(strlen(source)+3);
        **output=STR_MARK_NUM;
        strcpy((*output+1),source);
        int tempSize=strlen(*output);
        *(*output+tempSize)=STR_MARK_NUM;
        *(*output+tempSize+1)=0;
        ret=0;
    }
    return ret;
}
int Rtk_Ini::removeStrMark_needfree(char *source,char **output)
{
    int ret=-1;
    if(source)
    {
        char *line_buffer;
        char *begin,*end;
        line_buffer=strdup(source);
        if((begin=strstr(line_buffer,STR_MARK))!=NULL)
        {
            if((end=strrchr(line_buffer,STR_MARK_NUM))!=NULL)  
            {
                if(begin != end)
                {
                    *end=0;
                    *output=strdup(begin+1);
                    ret=0;
                }
            }
        }
        FREE(line_buffer);
    }
    return ret;
}
int Rtk_Ini::removeInsideValueStrMark()
{
    if(m_SectionHeader)
    {
        LLSection *TempLLSection;
        TempLLSection=m_SectionHeader;
        while(1)
        {
            if(TempLLSection)
            {
                LLKeyValue *TempLLKeyValue;
                if((TempLLKeyValue=TempLLSection->getHeadKeyValue())!=NULL)
                {
                    while(1)
                    {
                        if(TempLLKeyValue)
                        {
                            char *pInKey,*pInValue;
                            TempLLKeyValue->getKeyValue(&pInKey,&pInValue);
                            if(pInValue)
                            {
                                char *InNewValue=NULL,*InNewKey=NULL;
                                if(!removeStrMark_needfree(pInValue,&InNewValue))
                                {
                                    InNewKey=strdup(pInKey);
                                    TempLLKeyValue->setKeyValue(InNewKey,InNewValue);
                                    FREE(InNewValue);
                                    FREE(InNewKey);
                                }
                            }
                            TempLLKeyValue=TempLLKeyValue->getNextKeyValue();
                        }
                        else
                            break;
                    }
                }
                TempLLSection=TempLLSection->getNextSection();
            }
            else
                break;
        }
    }
    return 0;
}

// private private private private private private private private private private
// private private private private private private private private private private
// private private private private private private private private private private
Rtk_Ini::LLSection *Rtk_Ini::findSection(char *section)
{
    LLSection *ret=NULL;
    
    if(m_SectionBuffer)
    {
        char *sectionBufferName;
        m_SectionBuffer->getSectionName(&sectionBufferName);
        if(sectionBufferName && !strcmp(sectionBufferName,section))
            ret = m_SectionBuffer;
        else if(m_SectionBuffer->getNextSection())
        {
            m_SectionBuffer->getNextSection()->getSectionName(&sectionBufferName);
            if(sectionBufferName && !strcmp(sectionBufferName,section))
            {
                m_SectionBuffer=m_SectionBuffer->getNextSection();
                ret = m_SectionBuffer;
            }
        }
    }
    if(!ret && m_SectionHeader && section)
    {
        LLSection *TempSection;
        TempSection=m_SectionHeader;
        while(1)
        {
            char *pInSection;
            if(TempSection)
            {
                TempSection->getSectionName(&pInSection);
                if(pInSection != NULL && strcmp(section,pInSection)==0)
                {
                    ret=TempSection;
                    m_SectionBuffer=TempSection;
                    break;
                }
                else
                    TempSection=TempSection->getNextSection();
            }
            else
                break;
        }
    }
    return ret;
}
Rtk_Ini::LLKeyValue *Rtk_Ini::findKeyValue(Rtk_Ini::LLSection *pLLSection,char *key)
{
    LLKeyValue *ret=NULL;
    if(pLLSection && key)
    {
        LLKeyValue *TempKeyValue;
        TempKeyValue=pLLSection->getHeadKeyValue();
        while(1)
        {
            char *pInKey,*pInValue;
            if(TempKeyValue)
            {
                TempKeyValue->getKeyValue(&pInKey,&pInValue);
                if(pInKey != NULL && strcmp(key,pInKey)==0)
                {
                    ret=TempKeyValue;
                    break;
                }
                else
                    TempKeyValue=TempKeyValue->getNextKeyValue();
            }
            else
                break;
        }
    }
    return ret;
}
Rtk_Ini::LLKeyValue *Rtk_Ini::findKeyValue(char *section,char *key)
{
    LLKeyValue *ret=NULL;
    LLSection *TempLLSection;
    if((TempLLSection=findSection(section)) != NULL)
    {
        LLKeyValue *TempKeyValue;
        if((TempKeyValue=findKeyValue(TempLLSection,key)) != NULL)
        {
            ret=TempKeyValue;
        }
    }
    return ret;
}
Rtk_Ini::LLSection *Rtk_Ini::findLastSection(void)
{
    LLSection *ret=NULL;
    if(m_SectionBuffer && !m_SectionBuffer->getNextSection())
    {
        ret = m_SectionBuffer;
    }
    if(!ret && m_SectionHeader)
    {
        LLSection *TempSection,*TempLastSection=NULL;
        TempSection=m_SectionHeader;
        while(1)
        {
            if(TempSection)
            {
                TempLastSection=TempSection;
                TempSection=TempLastSection->getNextSection();
            }
            else
            {
                ret=TempLastSection;
                m_SectionBuffer=TempLastSection;
                break;
            }
        }
    }
    return ret;
}
Rtk_Ini::LLKeyValue *Rtk_Ini::findLastKeyValue(LLSection *pLLSection)
{
    LLKeyValue *ret=NULL;
    if(pLLSection->getHeadKeyValue())
    {
        LLKeyValue *TempKeyValue,*TempLastKeyValue=NULL;
        TempKeyValue=pLLSection->getHeadKeyValue();
        while(1)
        {
            if(TempKeyValue)
            {
                TempLastKeyValue=TempKeyValue;
                TempKeyValue=TempLastKeyValue->getNextKeyValue();
            }
            else
            {
                ret=TempLastKeyValue;
                break;
            }
        }
    }
    return ret;
}
Rtk_Ini::LLSection *Rtk_Ini::appendSection(char *section)
{
    LLSection *ret=NULL;
    LLSection *LastLLSection=NULL;
    if((LastLLSection=findLastSection())==NULL)
    {
        m_SectionHeader=new LLSection(section);
        ret=m_SectionHeader;
    }
    else
    {
        if(!LastLLSection->setNextSection(section))
            ret=LastLLSection->getNextSection();
    }
    if(ret)
        m_SectionBuffer=ret;
    return ret;
}
Rtk_Ini::LLKeyValue *Rtk_Ini::appendKeyValue(LLSection *pLLSection,char *key,char *value)
{
    LLKeyValue *ret=NULL;
    LLKeyValue *LastLLKeyValue=NULL;
    if((LastLLKeyValue=findLastKeyValue(pLLSection))==NULL)
    {
        if(!pLLSection->setHeadKeyValue(key, value))
            ret=pLLSection->getHeadKeyValue();
    }
    else
    {
        if(!LastLLKeyValue->setNextKeyValue(key, value))
            ret=LastLLKeyValue->getNextKeyValue();
    }
    return ret;
}
int Rtk_Ini::getSection_needfree(char *line,char **section)
{
    if(strstr(line,"id")!=NULL)	// fix bug 30362 ,for ssid may contains [,] 
	return -1;

    int ret=0;
    char *begin,*end;
    if((begin=strstr(line,"["))!=NULL)
    {
        begin++;
        if((end=strstr(line,"]"))!=NULL)
        {
            int section_size=end-begin+1;
            *section=(char *)malloc(section_size);
            memset(*section,0,section_size);
            memcpy(*section,begin,end-begin);
        }
        else
            ret=-1;
    }
    else
        ret=-1;
    return ret;
}
int Rtk_Ini::getKeyValue_needfree(char *line,char **key,char **value)
{
    int ret=0;
    char *separate;
    int length=strlen(line);
    *key=NULL;
    *value=NULL;
    jsprintf("[%s,%s,%d]line = %s \n",__FILE__,__func__,__LINE__,line);
    if((separate=strstr(line,STR_EQUL))!=NULL)
    {
        char *line_buffer;
        char *begin=NULL,*end=NULL;
        //get key
        line_buffer=strdup(line);
        *(strstr(line_buffer,STR_EQUL))=0;
        if((begin=strstr(line_buffer,STR_MARK))!=NULL)
        {
            if((end=strrchr(line_buffer,STR_MARK_NUM))!=NULL)  // 39: '
            {
                if(begin != end)
                {
                    *(end+1)=0;
                    *key=strdup(begin);
                }
                else
                    begin=NULL;
            }
            else
                begin=NULL;
        }
        else if(begin == NULL)
        {
            if(separate>line)
            {
                for(char *pChar=line;pChar<=separate;pChar++)
                {
                    if(*pChar >= 33 && *pChar <=126)  //Visable  33:! , 126 :~
                    {
                        begin=pChar;
                        break;
                    }
                }
                jsprintf("[%s,%s,%d]line = %s ,begin = %s\n",__FILE__,__func__,__LINE__,line,begin);
                for(char *pChar=separate-1;pChar>=begin;pChar--)
                {
                    if(*pChar >= 33 && *pChar <=126)  //Visable  33:! , 126 :~
                    {
                        end=pChar;
                        break;
                    }
                }
                jsprintf("[%s,%s,%d]line = %s ,begin = %s ,end = %s ,\n",__FILE__,__func__,__LINE__,line,begin,end);
            }
            if(begin && end)
            {
                int key_size=end-begin+2;
                *key=(char *)malloc(key_size);
                memset(*key,0,key_size);
                memcpy(*key,begin,end-begin+1);
            }
            else
                *key=strdup(STR_NULL);
        }
        FREE(line_buffer);
        //get value
        begin=NULL;
        end=NULL;
        line_buffer=strdup(separate+1);
        if((begin=strstr(line_buffer,STR_MARK))!=NULL)
        {
            if((end=strrchr(line_buffer,STR_MARK_NUM))!=NULL) // 39: '
            {
                if(begin != end)
                {
                    *(end+1)=0;
                    *value=strdup(begin);
                }
                else
                    begin=NULL;
            }
            else
                begin=NULL;
        }
        else if(begin == NULL)
        {
            if(line+length >separate)
            {
                for(char *pChar=separate+1;pChar<=(line+length);pChar++)
                {
                    if(*pChar >= 33 && *pChar <=126)  //Visable  33:! , 126 :~
                    {
                        begin=pChar;
                        break;
                    }
                }
                for(char *pChar=(line+length);pChar>=begin;pChar--)
                {
                    if(*pChar >= 33 && *pChar <=126)  //Visable  33:! , 126 :~
                    {
                        end=pChar;
                        break;
                    }
                }
            }
            jsprintf("[%s,%s,%d]line = %s ,begin = %s ,end = %s ,\n",__FILE__,__func__,__LINE__,line,begin,end);
            if(begin && end)
            {
                int value_size=end-begin+2;
                *value=(char *)malloc(value_size);
                memset(*value,0,value_size);
                memcpy(*value,begin,end-begin+1);
            }
            else
                *value=strdup(STR_NULL);
        }
        FREE(line_buffer);
    }
    else
        ret=-1;
    return ret;
}


