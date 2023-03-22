#ifndef UBTSIGN_H
#define UBTSIGN_H

#include <string>
#include <stdint.h>

class UBTSign
{
public:
    UBTSign();
    ~UBTSign();
    std::string generateRandSignSeed();
    std::string getHeaderXUBTSignV3(const std::string &deviceId);
    std::string getURestSign (const std::string& strSNNum);
    std::string createVerifyCode (int nBits);
    uint64_t GetCurrentTimerMS();
    std::string GetBasicParam(int nType, std::string strVersion);
    std::string GetCurrentTimerStringMs();

private:
    std::string appKey_;
};

#endif // UBTSIGN_H
