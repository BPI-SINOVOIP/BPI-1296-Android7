#ifndef __RTK_INI_H__
#define __RTK_INI_H__
#include <sys/types.h>
class Rtk_Ini
{
public:
    Rtk_Ini();
    ~Rtk_Ini();
    int loadFile(char *filename);
    int saveFile(char *filename);
    char *getStrValue(char *section,char *key);
    int setStrValue(char *section,char *key,char *value);
    int getIntValue(char *section,char *key,int *value);
    int setIntValue(char *section,char *key,int value);
    int getU64IntValue(char *section,char *key,u_int64_t *value);
    int setU64IntValue(char *section,char *key,u_int64_t value);
    int addStrMark_needfree(char *source,char **output);
    int removeStrMark_needfree(char *source,char **output);
    int removeInsideValueStrMark();// for internal Object
private:
    class LLKeyValue
    {
         public:
            LLKeyValue(char *key,char *value);
            ~LLKeyValue();
            int getKeyValue(char **key,char **value);
            int setKeyValue(char *key,char *value);
            LLKeyValue *getNextKeyValue();
            int setNextKeyValue(char *key,char *value);
        private:
            char *m_pKey;
            char *m_pValue;
            LLKeyValue *m_NextKeyValue;
    };
    class LLSection
    {
         public:
            LLSection(char *section);
            ~LLSection();
            int getSectionName(char **section);
            int setSectionName(char *section);
            LLKeyValue *getHeadKeyValue();
            int setHeadKeyValue(char *key,char *value);
            LLSection *getNextSection();
            int setNextSection(char *section);
        private:
            char *m_pSection;
            LLSection *m_Next_Section;
            LLKeyValue *m_HeadKeyValue;
    };
    LLSection *m_SectionHeader;
    LLSection *m_SectionBuffer;
    LLSection *findSection(char *section);
    LLKeyValue *findKeyValue(LLSection *pLLSection,char *key);
    LLKeyValue *findKeyValue(char *section,char *key);
    LLSection *findLastSection(void);
    LLKeyValue *findLastKeyValue(LLSection *pLLSection);
    LLSection *appendSection(char *section);
    LLKeyValue *appendKeyValue(LLSection *pLLSection,char *key,char *value);
    int getSection_needfree(char *line,char **section);
    int getKeyValue_needfree(char *line,char **key,char **value);
};


#endif

