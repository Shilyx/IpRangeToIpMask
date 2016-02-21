/*
 *  @file  : IpRangeToIpMask.cpp
 *  @author: luteng
 *  @date  : 2015-04-29 15:40:24.291
 *  @note  : Generated by SlxTemplates
 */

#include <WinSock2.h>
#include <stdio.h>
#pragma warning(disable: 4786)
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<pair<unsigned, unsigned> > IpRangeToIpMask(unsigned uBeginNetwork, unsigned uEndNetwork)
{
    class Local
    {
    public:
        static bool IpRangeToIpMask(unsigned uBeginHost, unsigned uEndHost, vector<pair<unsigned, unsigned> > &resultNetwork)
        {
            if (uBeginHost > uEndHost)
            {
                return false;
            }
            else if (uBeginHost == uEndHost)
            {
                resultNetwork.push_back(make_pair(htonl(uBeginHost), -1));
                return true;
            }
            else
            {
                DWORD dwSeed = -1;

                for (int i = 0; i <= 32; ++i, dwSeed >>= 1)
                {
                    unsigned uMaskHost = ~dwSeed;

                    if ((uBeginHost & uMaskHost) == uBeginHost)
                    {
                        unsigned uEndHostNow = uBeginHost | ~uMaskHost;

                        if (uEndHostNow <= uEndHost)
                        {
                            resultNetwork.push_back(make_pair(htonl(uBeginHost), htonl(uMaskHost)));
                            return true;
                        }
                    }
                }

                // we never got here
                return false;
            }
        }
    };

    vector<pair<unsigned, unsigned> > result;
    unsigned uBeginHost = ntohl(uBeginNetwork);
    unsigned uEndHost = ntohl(uEndNetwork);

    while (Local::IpRangeToIpMask(uBeginHost, uEndHost, result))
    {
        unsigned uMaskHost = ntohl(result.rbegin()->second);

        uBeginHost |= ~uMaskHost;

        if (uBeginHost >= uEndHost)
        {
            break;
        }

        uBeginHost += 1;
    }

    return result;
}

int main(int argc, char *argv[])
{
    vector<pair<unsigned, unsigned> > v = IpRangeToIpMask(
        inet_addr("36.96.0.0"),
        inet_addr("36.223.255.222"));

    for (vector<pair<unsigned, unsigned> >::const_iterator it = v.begin(); it != v.end(); ++it)
    {
        string str1 = inet_ntoa(*(in_addr *)&it->first);
        string str2 = inet_ntoa(*(in_addr *)&it->second);

        cout<<str1<<" - "<<str2<<endl;
    }

    return 0;
}