/*
 *	Copyright (C) 2003-2006 Gabest
 *	http://www.gabest.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GNU Make; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "stdafx.h"
#include "STS.h"
#include <atlbase.h>
#include <atlconv.h>
#include <atlstr.h>

#include <fstream>

static long revcolor(long c)
{
    return ((c & 0xff0000) >> 16) + (c & 0xff00) + ((c & 0xff) << 16);
}

// gathered from http://www.netwave.or.jp/~shikai/shikai/shcolor.htm

struct htmlcolor
{
    TCHAR* name;
    DWORD color;
} hmtlcolors[] =
{
    {_T("white"), 0xffffff},
    {_T("whitesmoke"), 0xf5f5f5},
    {_T("ghostwhite"), 0xf8f8ff},
    {_T("snow"), 0xfffafa},
    {_T("gainsboro"), 0xdcdcdc},
    {_T("lightgrey"), 0xd3d3d3},
    {_T("silver"), 0xc0c0c0},
    {_T("darkgray"), 0xa9a9a9},
    {_T("gray"), 0x808080},
    {_T("dimgray"), 0x696969},
    {_T("lightslategray"), 0x778899},
    {_T("slategray"), 0x708090},
    {_T("darkslategray"), 0x2f4f4f},
    {_T("black"), 0x000000},

    {_T("azure"), 0xf0ffff},
    {_T("aliceblue"), 0xf0f8ff},
    {_T("mintcream"), 0xf5fffa},
    {_T("honeydew"), 0xf0fff0},
    {_T("lightcyan"), 0xe0ffff},
    {_T("paleturqoise"), 0xafeeee},
    {_T("powderblue"), 0xb0e0e6},
    {_T("lightblue"), 0xadd8ed},
    {_T("lightsteelblue"), 0xb0c4de},
    {_T("skyblue"), 0x87ceeb},
    {_T("lightskyblue"), 0x87cefa},
    {_T("cyan"), 0x00ffff},
    {_T("aqua"), 0x00ff80},
    {_T("deepskyblue"), 0x00bfff},
    {_T("aquamarine"), 0x7fffd4},
    {_T("turquoise"), 0x40e0d0},
    {_T("darkturquoise"), 0x00ced1},
    {_T("lightseagreen"), 0x20b2aa},
    {_T("mediumturquoise"), 0x40e0dd},
    {_T("mediumaquamarine"), 0x66cdaa},
    {_T("cadetblue"), 0x5f9ea0},
    {_T("teal"), 0x008080},
    {_T("darkcyan"), 0x008b8b},
    {_T("comflowerblue"), 0x6495ed},
    {_T("dodgerblue"), 0x1e90ff},
    {_T("steelblue"), 0x4682b4},
    {_T("royalblue"), 0x4169e1},
    {_T("blue"), 0x0000ff},
    {_T("mediumblue"), 0x0000cd},
    {_T("mediumslateblue"), 0x7b68ee},
    {_T("slateblue"), 0x6a5acd},
    {_T("darkslateblue"), 0x483d8b},
    {_T("darkblue"), 0x00008b},
    {_T("midnightblue"), 0x191970},
    {_T("navy"), 0x000080},

    {_T("palegreen"), 0x98fb98},
    {_T("lightgreen"), 0x90ee90},
    {_T("mediumspringgreen"), 0x00fa9a},
    {_T("springgreen"), 0x00ff7f},
    {_T("chartreuse"), 0x7fff00},
    {_T("lawngreen"), 0x7cfc00},
    {_T("lime"), 0x00ff00},
    {_T("limegreen"), 0x32cd32},
    {_T("greenyellow"), 0xadff2f},
    {_T("yellowgreen"), 0x9acd32},
    {_T("darkseagreen"), 0x8fbc8f},
    {_T("mediumseagreen"), 0x3cb371},
    {_T("seagreen"), 0x2e8b57},
    {_T("olivedrab"), 0x6b8e23},
    {_T("forestgreen"), 0x228b22},
    {_T("green"), 0x008000},
    {_T("darkkhaki"), 0xbdb76b},
    {_T("olive"), 0x808000},
    {_T("darkolivegreen"), 0x556b2f},
    {_T("darkgreen"), 0x006400},

    {_T("floralwhite"), 0xfffaf0},
    {_T("seashell"), 0xfff5ee},
    {_T("ivory"), 0xfffff0},
    {_T("beige"), 0xf5f5dc},
    {_T("cornsilk"), 0xfff8dc},
    {_T("lemonchiffon"), 0xfffacd},
    {_T("lightyellow"), 0xffffe0},
    {_T("lightgoldenrodyellow"), 0xfafad2},
    {_T("papayawhip"), 0xffefd5},
    {_T("blanchedalmond"), 0xffedcd},
    {_T("palegoldenrod"), 0xeee8aa},
    {_T("khaki"), 0xf0eb8c},
    {_T("bisque"), 0xffe4c4},
    {_T("moccasin"), 0xffe4b5},
    {_T("navajowhite"), 0xffdead},
    {_T("peachpuff"), 0xffdab9},
    {_T("yellow"), 0xffff00},
    {_T("gold"), 0xffd700},
    {_T("wheat"), 0xf5deb3},
    {_T("orange"), 0xffa500},
    {_T("darkorange"), 0xff8c00},

    {_T("oldlace"), 0xfdf5e6},
    {_T("linen"), 0xfaf0e6},
    {_T("antiquewhite"), 0xfaebd7},
    {_T("lightsalmon"), 0xffa07a},
    {_T("darksalmon"), 0xe9967a},
    {_T("salmon"), 0xfa8072},
    {_T("lightcoral"), 0xf08080},
    {_T("indianred"), 0xcd5c5c},
    {_T("coral"), 0xff7f50},
    {_T("tomato"), 0xff6347},
    {_T("orangered"), 0xff4500},
    {_T("red"), 0xff0000},
    {_T("crimson"), 0xdc143c},
    {_T("firebrick"), 0xb22222},
    {_T("maroon"), 0x800000},
    {_T("darkred"), 0x8b0000},

    {_T("lavender"), 0xe6e6fe},
    {_T("lavenderblush"), 0xfff0f5},
    {_T("mistyrose"), 0xffe4e1},
    {_T("thistle"), 0xd8bfd8},
    {_T("pink"), 0xffc0cb},
    {_T("lightpink"), 0xffb6c1},
    {_T("palevioletred"), 0xdb7093},
    {_T("hotpink"), 0xff69b4},
    {_T("fuchsia"), 0xff00ee},
    {_T("magenta"), 0xff00ff},
    {_T("mediumvioletred"), 0xc71585},
    {_T("deeppink"), 0xff1493},
    {_T("plum"), 0xdda0dd},
    {_T("violet"), 0xee82ee},
    {_T("orchid"), 0xda70d6},
    {_T("mediumorchid"), 0xba55d3},
    {_T("mediumpurple"), 0x9370db},
    {_T("purple"), 0x9370db},
    {_T("blueviolet"), 0x8a2be2},
    {_T("darkviolet"), 0x9400d3},
    {_T("darkorchid"), 0x9932cc},

    {_T("tan"), 0xd2b48c},
    {_T("burlywood"), 0xdeb887},
    {_T("sandybrown"), 0xf4a460},
    {_T("peru"), 0xcd853f},
    {_T("goldenrod"), 0xdaa520},
    {_T("darkgoldenrod"), 0xb8860b},
    {_T("chocolate"), 0xd2691e},
    {_T("rosybrown"), 0xbc8f8f},
    {_T("sienna"), 0xa0522d},
    {_T("saddlebrown"), 0x8b4513},
    {_T("brown"), 0xa52a2a},
};

CHtmlColorMap::CHtmlColorMap()
{
    for(ptrdiff_t i = 0; i < countof(hmtlcolors); i++)
        SetAt(hmtlcolors[i].name, hmtlcolors[i].color);
}

CHtmlColorMap g_colors;

//

BYTE CharSetList[] =
{
    ANSI_CHARSET,
    DEFAULT_CHARSET,
    SYMBOL_CHARSET,
    SHIFTJIS_CHARSET,
    HANGEUL_CHARSET,
    HANGUL_CHARSET,
    GB2312_CHARSET,
    CHINESEBIG5_CHARSET,
    OEM_CHARSET,
    JOHAB_CHARSET,
    HEBREW_CHARSET,
    ARABIC_CHARSET,
    GREEK_CHARSET,
    TURKISH_CHARSET,
    VIETNAMESE_CHARSET,
    THAI_CHARSET,
    EASTEUROPE_CHARSET,
    RUSSIAN_CHARSET,
    MAC_CHARSET,
    BALTIC_CHARSET
};

TCHAR* CharSetNames[] =
{
    _T("ANSI"),
    _T("DEFAULT"),
    _T("SYMBOL"),
    _T("SHIFTJIS"),
    _T("HANGEUL"),
    _T("HANGUL"),
    _T("GB2312"),
    _T("CHINESEBIG5"),
    _T("OEM"),
    _T("JOHAB"),
    _T("HEBREW"),
    _T("ARABIC"),
    _T("GREEK"),
    _T("TURKISH"),
    _T("VIETNAMESE"),
    _T("THAI"),
    _T("EASTEUROPE"),
    _T("RUSSIAN"),
    _T("MAC"),
    _T("BALTIC"),
};

int CharSetLen = countof(CharSetList);

//

static DWORD CharSetToCodePage(DWORD dwCharSet)
{
    CHARSETINFO cs = {0};
    ::TranslateCharsetInfo((DWORD *)dwCharSet, &cs, TCI_SRCCHARSET);
    return cs.ciACP;
}

int FindChar(CStringW str, WCHAR c, int pos, bool fUnicode, int CharSet)
{
    if(fUnicode) return(str.Find(c, pos));

    int fStyleMod = 0;

    DWORD cp = CharSetToCodePage(CharSet);
    int OrgCharSet = CharSet;

    for(size_t i = 0, j = str.GetLength(), k; i < j; i++)
    {
        WCHAR c2 = str[i];

        if(IsDBCSLeadByteEx(cp, (BYTE)c2)) i++;
        else if(i >= pos)
        {
            if(c2 == c) return(i);
        }

        if(c2 == '{') fStyleMod++;
        else if(fStyleMod > 0)
        {
            if(c2 == '}') fStyleMod--;
            else if(c2 == 'e' && i >= 3 && i < j - 1 && str.Mid(i - 2, 3) == L"\\fe")
            {
                CharSet = 0;
                for(k = i + 1; _istdigit(str[k]); k++) CharSet = CharSet * 10 + (str[k] - '0');
                if(k == i + 1) CharSet = OrgCharSet;

                cp = CharSetToCodePage(CharSet);
            }
        }
    }

    return(-1);
}
/*
int FindChar(CStringA str, char c, int pos, bool fUnicode, int CharSet)
{
    ASSERT(!fUnicode);

    return(FindChar(AToW(str), c, pos, false, CharSet));
}
*/
static CStringW ToMBCS(CStringW str, DWORD CharSet)
{
    CStringW ret;

    DWORD cp = CharSetToCodePage(CharSet);

    for(ptrdiff_t i = 0, j = str.GetLength(); i < j; i++)
    {
        WCHAR wc = str.GetAt(i);
        char c[8];

        int len;
        if((len = WideCharToMultiByte(cp, 0, &wc, 1, c, 8, NULL, NULL)) > 0)
        {
            for(ptrdiff_t k = 0; k < len; k++)
                ret += (WCHAR)(BYTE)c[k];
        }
        else
        {
            ret += '?';
        }
    }

    return(ret);
}

static CStringW UnicodeSSAToMBCS(CStringW str, DWORD CharSet)
{
    CStringW ret;

    int OrgCharSet = CharSet;

    for(ptrdiff_t j = 0; j < str.GetLength();)
    {
        j = str.Find('{', j);
        if(j >= 0)
        {
            ret += ToMBCS(str.Left(j), CharSet);
            str = str.Mid(j);

            j = str.Find('}');
            if(j < 0)
            {
                ret += ToMBCS(str, CharSet);
                break;
            }
            else
            {
                int k = str.Find(L"\\fe");
                if(k >= 0 && k < j)
                {
                    CharSet = 0;
                    int l = k + 3;
                    for(; _istdigit(str[l]); l++) CharSet = CharSet * 10 + (str[l] - '0');
                    if(l == k + 3) CharSet = OrgCharSet;
                }

                j++;

                ret += ToMBCS(str.Left(j), OrgCharSet);
                str = str.Mid(j);
                j = 0;
            }
        }
        else
        {
            ret += ToMBCS(str, CharSet);
            break;
        }
    }

    return(ret);
}

static CStringW ToUnicode(CStringW str, DWORD CharSet)
{
    CStringW ret;

    DWORD cp = CharSetToCodePage(CharSet);

    for(ptrdiff_t i = 0, j = str.GetLength(); i < j; i++)
    {
        WCHAR wc = str.GetAt(i);
        char c = wc & 0xff;

        if(IsDBCSLeadByteEx(cp, (BYTE)wc))
        {
            i++;

            if(i < j)
            {
                char cc[2];
                cc[0] = c;
                cc[1] = (char)str.GetAt(i);

                MultiByteToWideChar(cp, 0, cc, 2, &wc, 1);
            }
        }
        else
        {
            MultiByteToWideChar(cp, 0, &c, 1, &wc, 1);
        }

        ret += wc;
    }

    return(ret);
}

static CStringW MBCSSSAToUnicode(CStringW str, int CharSet)
{
    CStringW ret;

    int OrgCharSet = CharSet;

    for(ptrdiff_t j = 0; j < str.GetLength();)
    {
        j = FindChar(str, '{', 0, false, CharSet);

        if(j >= 0)
        {
            ret += ToUnicode(str.Left(j), CharSet);
            str = str.Mid(j);

            j = FindChar(str, '}', 0, false, CharSet);

            if(j < 0)
            {
                ret += ToUnicode(str, CharSet);
                break;
            }
            else
            {
                int k = str.Find(L"\\fe");
                if(k >= 0 && k < j)
                {
                    CharSet = 0;
                    int l = k + 3;
                    for(; _istdigit(str[l]); l++) CharSet = CharSet * 10 + (str[l] - '0');
                    if(l == k + 3) CharSet = OrgCharSet;
                }

                j++;

                ret += ToUnicode(str.Left(j), OrgCharSet);
                str = str.Mid(j);
                j = 0;
            }
        }
        else
        {
            ret += ToUnicode(str, CharSet);
            break;
        }
    }

    return(ret);
}

CStringW RemoveSSATags(CStringW str, bool fUnicode, int CharSet)
{
    str.Replace(L"{\\i1}", L"<i>");
    str.Replace(L"{\\i}", L"</i>");

    for(ptrdiff_t i = 0, j; i < str.GetLength();)
    {
        if((i = FindChar(str, '{', i, fUnicode, CharSet)) < 0) break;
        if((j = FindChar(str, '}', i, fUnicode, CharSet)) < 0) break;
        str.Delete(i, j - i + 1);
    }

    str.Replace(L"\\N", L"\n");
    str.Replace(L"\\n", L"\n");
    str.Replace(L"\\h", L" ");

    return(str);
}

//

static CStringW SubRipper2SSA(CStringW str, int CharSet)
{
    str.Replace(L"<i>", L"{\\i1}");
    str.Replace(L"</i>", L"{\\i}");
    str.Replace(L"<b>", L"{\\b1}");
    str.Replace(L"</b>", L"{\\b}");
    str.Replace(L"<u>", L"{\\u1}");
    str.Replace(L"</u>", L"{\\u}");

    return(str);
}

static bool OpenSubRipper(CTextFile* file, CSimpleTextSubtitle& ret, int CharSet)
{
    int num = 0;

    CStringW buff;
    while(file->ReadString(buff))
    {
        buff.Trim();
        if(buff.IsEmpty()) continue;

        WCHAR sep;
        int hh1, mm1, ss1, ms1, hh2, mm2, ss2, ms2;
        int c = swscanf(buff, L"%d%c%d%c%d%c%d --> %d%c%d%c%d%c%d\n",
                        &hh1, &sep, &mm1, &sep, &ss1, &sep, &ms1,
                        &hh2, &sep, &mm2, &sep, &ss2, &sep, &ms2);

        if(c == 1) // numbering
        {
            num = hh1;
        }
        else if(c == 14) // time info
        {
            CStringW str, tmp;

            bool fFoundEmpty = false;

            while(file->ReadString(tmp))
            {
                tmp.Trim();
                if(tmp.IsEmpty()) fFoundEmpty = true;

                int num2;
                WCHAR c;
                if(swscanf(tmp, L"%d%c", &num2, &c) == 1 && fFoundEmpty)
                {
                    num = num2;
                    break;
                }

                str += tmp + '\n';
            }

            ret.Add(
                SubRipper2SSA(str, CharSet),
                file->IsUnicode(),
                (((hh1 * 60 + mm1) * 60) + ss1) * 1000 + ms1,
                (((hh2 * 60 + mm2) * 60) + ss2) * 1000 + ms2);
        }
        else if(c != EOF) // might be another format
        {
            return(false);
        }
    }

    return(ret.GetCount() > 0);
}

static void ReplaceNoCase(CStringW& str, CStringW from, CStringW to)
{
    CStringW lstr = str;
    lstr.MakeLower();

    int i, j, k;

    for(i = 0, j = str.GetLength(); i < j;)
    {
        if((k = lstr.Find(from, i)) >= 0)
        {
            str.Delete(k, from.GetLength());
            lstr.Delete(k, from.GetLength());
            str.Insert(k, to);
            lstr.Insert(k, to);
            i = k + to.GetLength();
            j = str.GetLength();
        }
        else break;
    }
}

CStringW GetStr(CStringW& buff, char sep = ',') //throw(...)
{
    buff.TrimLeft();

    int pos = buff.Find(sep);
    if(pos < 0)
    {
        pos = buff.GetLength();
        if(pos < 1) throw 1;
    }

    CStringW ret = buff.Left(pos);
    if(pos < buff.GetLength()) buff = buff.Mid(pos + 1);

    return(ret);
}

int GetInt(CStringW& buff, char sep = ',') //throw(...)
{
    CStringW str;

    str = GetStr(buff, sep);
    str.MakeLower();

    CStringW fmtstr = str.GetLength() > 2 && (str.Left(2) == L"&h" || str.Left(2) == L"0x")
                      ? str = str.Mid(2), L"%x"
                              : L"%d";

    int ret;
    if(swscanf(str, fmtstr, &ret) != 1) throw 1;

    return(ret);
}

double GetFloat(CStringW& buff, char sep = ',') //throw(...)
{
    CStringW str;

    str = GetStr(buff, sep);
    str.MakeLower();

    float ret;
    if(swscanf(str, L"%f", &ret) != 1) throw 1;

    return((double)ret);
}

static bool LoadFont(CString& font)
{
    int len = font.GetLength();

    CAutoVectorPtr<BYTE> pData;
    if(len == 0 || (len & 3) == 1 || !pData.Allocate(len))
        return(false);

    const TCHAR* s = font;
    const TCHAR* e = s + len;
    for(BYTE* p = pData; s < e; s++, p++) *p = *s - 33;

    for(ptrdiff_t i = 0, j = 0, k = len&~3; i < k; i += 4, j += 3)
    {
        pData[j+0] = ((pData[i+0] & 63) << 2) | ((pData[i+1] >> 4) & 3);
        pData[j+1] = ((pData[i+1] & 15) << 4) | ((pData[i+2] >> 2) & 15);
        pData[j+2] = ((pData[i+2] & 3) << 6) | ((pData[i+3] >> 0) & 63);
    }

    int datalen = (len&~3) * 3 / 4;

    if((len & 3) == 2)
    {
        pData[datalen++] = ((pData[(len&~3)+0] & 63) << 2) | ((pData[(len&~3)+1] >> 4) & 3);
    }
    else if((len & 3) == 3)
    {
        pData[datalen++] = ((pData[(len&~3)+0] & 63) << 2) | ((pData[(len&~3)+1] >> 4) & 3);
        pData[datalen++] = ((pData[(len&~3)+1] & 15) << 4) | ((pData[(len&~3)+2] >> 2) & 15);
    }

    HANDLE hFont = INVALID_HANDLE_VALUE;

    if(HMODULE hModule = LoadLibrary(_T("GDI32.DLL")))
    {
        typedef HANDLE(WINAPI * PAddFontMemResourceEx)(IN PVOID, IN DWORD, IN PVOID , IN DWORD*);
        if(PAddFontMemResourceEx f = (PAddFontMemResourceEx)GetProcAddress(hModule, "AddFontMemResourceEx"))
        {
            DWORD cFonts;
            hFont = f(pData, datalen, NULL, &cFonts);
        }

        FreeLibrary(hModule);
    }

    if(hFont == INVALID_HANDLE_VALUE)
    {
        TCHAR path[_MAX_PATH];
        GetTempPath(_MAX_PATH, path);

        DWORD chksum = 0;
        for(ptrdiff_t i = 0, j = datalen >> 2; i < j; i++)
            chksum += ((DWORD*)(BYTE*)pData)[i];

        CString fn;
        fn.Format(_T("%sfont%08x.ttf"), path, chksum);

        CFileStatus	fs;
        HRESULT r;
        try
        {
            r = CFile::GetStatus(fn, fs);
        }
        catch(CException* e)
        {
            // MFCBUG: E_INVALIDARG / "Parameter is incorrect" is thrown for certain cds (vs2003)
            // http://groups.google.co.uk/groups?hl=en&lr=&ie=UTF-8&threadm=OZuXYRzWDHA.536%40TK2MSFTNGP10.phx.gbl&rnum=1&prev=/groups%3Fhl%3Den%26lr%3D%26ie%3DISO-8859-1
            TRACE(_T("CFile::GetStatus has thrown an exception\n"));
            e->Delete();
            r = false;
        }

        if(!r)
        {
            CFile f;
            if(f.Open(fn, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone))
            {
                f.Write(pData, datalen);
                f.Close();
            }
        }

        AddFontResource(fn);
    }

    return(true);
}

static bool LoadUUEFont(CTextFile* file)
{
    CString s, font;
    while(file->ReadString(s))
    {
        s.Trim();
        if(s.IsEmpty()) break;
        if(s[0] == '[') // check for some standatr blocks
        {
            if(s.Find(_T("[Script Info]")) == 0) break;
            if(s.Find(_T("[V4+ Styles]")) == 0) break;
            if(s.Find(_T("[V4 Styles]")) == 0) break;
            if(s.Find(_T("[Events]")) == 0) break;
            if(s.Find(_T("[Fonts]")) == 0) break;
            if(s.Find(_T("[Graphics]")) == 0) break;
        }
        if(s.Find(_T("fontname:")) == 0)
        {
            LoadFont(font);
            font.Empty();
            continue;
        }

        font += s;
    }

    if(!font.IsEmpty())
        LoadFont(font);

    return(true);
}

#ifdef _VSMOD
bool CSimpleTextSubtitle::LoadEfile(CString& img, CString m_fn)
{
    int len = img.GetLength();

    CAutoVectorPtr<BYTE> pData;
    if(len == 0 || (len & 3) == 1 || !pData.Allocate(len))
        return(false);

    const TCHAR* s = img;
    const TCHAR* e = s + len;
    for(BYTE* p = pData; s < e; s++, p++) *p = *s - 33;

    for(ptrdiff_t i = 0, j = 0, k = len&~3; i < k; i += 4, j += 3)
    {
        pData[j+0] = ((pData[i+0] & 63) << 2) | ((pData[i+1] >> 4) & 3);
        pData[j+1] = ((pData[i+1] & 15) << 4) | ((pData[i+2] >> 2) & 15);
        pData[j+2] = ((pData[i+2] & 3) << 6) | ((pData[i+3] >> 0) & 63);
    }

    int datalen = (len&~3) * 3 / 4;

    if((len & 3) == 2)
    {
        pData[datalen++] = ((pData[(len&~3)+0] & 63) << 2) | ((pData[(len&~3)+1] >> 4) & 3);
    }
    else if((len & 3) == 3)
    {
        pData[datalen++] = ((pData[(len&~3)+0] & 63) << 2) | ((pData[(len&~3)+1] >> 4) & 3);
        pData[datalen++] = ((pData[(len&~3)+1] & 15) << 4) | ((pData[(len&~3)+2] >> 2) & 15);
    }

    // load png image
    MOD_PNGIMAGE t_temp;
    if(t_temp.initImage(pData.m_p, m_fn)) // save path
    {
        mod_images.Add(t_temp);
    }
    return(true);
}


bool CSimpleTextSubtitle::LoadUUEFile(CTextFile* file, CString m_fn)
{
    CString s, img;
    while(file->ReadString(s))
    {
        s.Trim();
        if(s.IsEmpty()) break;
        if(s[0] == '[') // check for some standatr blocks
        {
            if(s.Find(_T("[Script Info]")) == 0) break;
            if(s.Find(_T("[V4+ Styles]")) == 0) break;
            if(s.Find(_T("[V4 Styles]")) == 0) break;
            if(s.Find(_T("[Events]")) == 0) break;
            if(s.Find(_T("[Fonts]")) == 0) break;
            if(s.Find(_T("[Graphics]")) == 0) break;
        }
        // next file
        if(s.Find(_T("filename:")) == 0)
        {
            LoadEfile(img, m_fn);
            m_fn = s.Mid(10);
            img.Empty();
            continue;
        }

        img += s;
    }

    if(!img.IsEmpty())
        LoadEfile(img, m_fn);

    return(true);
}
#endif

static bool OpenSubStationAlpha(CTextFile* file, CSimpleTextSubtitle& ret, int CharSet)
{
    bool fRet = false;

    int version = 3, sver = 3;

    CStringW buff;
    while(file->ReadString(buff))
    {
        buff.Trim();
        if(buff.IsEmpty() || buff.GetAt(0) == ';') continue;

        CStringW entry;

//		try {
        entry = GetStr(buff, ':');
//	}
//		catch(...) {continue;}

        entry.MakeLower();

        if(entry == L"[script info]")
        {
            fRet = true;
        }
        else if(entry == L"playresx")
        {
            try
            {
                ret.m_dstScreenSize.cx = GetInt(buff);
            }
            catch(...)
            {
                ret.m_dstScreenSize = CSize(0, 0);
                return(false);
            }

            if(ret.m_dstScreenSize.cy <= 0)
            {
                ret.m_dstScreenSize.cy = (ret.m_dstScreenSize.cx == 1280)
                                         ? 1024
                                         : ret.m_dstScreenSize.cx * 3 / 4;
            }
        }
        else if(entry == L"playresy")
        {
            try
            {
                ret.m_dstScreenSize.cy = GetInt(buff);
            }
            catch(...)
            {
                ret.m_dstScreenSize = CSize(0, 0);
                return(false);
            }

            if(ret.m_dstScreenSize.cx <= 0)
            {
                ret.m_dstScreenSize.cx = (ret.m_dstScreenSize.cy == 1024)
                                         ? 1280
                                         : ret.m_dstScreenSize.cy * 4 / 3;
            }
        }
        else if(entry == L"wrapstyle")
        {
            try
            {
                ret.m_defaultWrapStyle = GetInt(buff);
            }
            catch(...)
            {
                ret.m_defaultWrapStyle = 1;
                return(false);
            }
        }
        else if(entry == L"scripttype")
        {
            if(buff.GetLength() >= 4 && !buff.Right(4).CompareNoCase(L"4.00")) version = sver = 4;
            else if(buff.GetLength() >= 5 && !buff.Right(5).CompareNoCase(L"4.00+")) version = sver = 5;
            else if(buff.GetLength() >= 6 && !buff.Right(6).CompareNoCase(L"4.00++")) version = sver = 6;
        }
        else if(entry == L"collisions")
        {
            buff = GetStr(buff);
            buff.MakeLower();
            ret.m_collisions = buff.Find(L"reverse") >= 0 ? 1 : 0;
        }
        else if(entry == L"scaledborderandshadow")
        {
            buff = GetStr(buff);
            buff.MakeLower();
            ret.m_fScaledBAS = buff.Find(L"yes") >= 0;
        }
        else if(entry == L"[v4 styles]")
        {
            fRet = true;
            sver = 4;
        }
        else if(entry == L"[v4+ styles]")
        {
            fRet = true;
            sver = 5;
        }
        else if(entry == L"[v4++ styles]")
        {
            fRet = true;
            sver = 6;
        }
        else if(entry == L"style")
        {
            STSStyle* style = new STSStyle;
            if(!style) return(false);

            try
            {
                CString StyleName;
                int alpha;

                StyleName = WToT(GetStr(buff));
                style->fontName = WToT(GetStr(buff));
                style->fontSize = GetFloat(buff);
                for(ptrdiff_t i = 0; i < 4; i++) style->colors[i] = (COLORREF)GetInt(buff);
                style->fontWeight = !!GetInt(buff) ? FW_BOLD : FW_NORMAL;
                style->fItalic = !!GetInt(buff);
                if(sver >= 5)	style->fUnderline = !!GetInt(buff);
                if(sver >= 5)	style->fStrikeOut = !!GetInt(buff);
                if(sver >= 5)	style->fontScaleX = GetFloat(buff);
                if(sver >= 5)	style->fontScaleY = GetFloat(buff);
                if(sver >= 5)	style->fontSpacing = GetFloat(buff);
                if(sver >= 5)	style->fontAngleZ = GetFloat(buff);
                if(sver >= 4)	style->borderStyle = GetInt(buff);
                style->outlineWidthX = style->outlineWidthY = GetFloat(buff);
                style->shadowDepthX = style->shadowDepthY = GetFloat(buff);
                style->scrAlignment = GetInt(buff);
                style->marginRect.left = GetInt(buff);
                style->marginRect.right = GetInt(buff);
                style->marginRect.top = style->marginRect.bottom = GetInt(buff);
                if(sver >= 6)	style->marginRect.bottom = GetInt(buff);
                if(sver <= 4)	alpha = GetInt(buff);
                style->charSet = GetInt(buff);
                if(sver >= 6)	style->relativeTo = GetInt(buff);

                if(sver <= 4)	style->colors[2] = style->colors[3]; // style->colors[2] is used for drawing the outline
                if(sver <= 4)	alpha = max(min(alpha, 0xff), 0);
                if(sver <= 4)
                {
                    for(ptrdiff_t i = 0; i < 3; i++) style->alpha[i] = alpha;
                    style->alpha[3] = 0x80;
                }
                if(sver >= 5)	for(ptrdiff_t i = 0; i < 4; i++)
                    {
                        style->alpha[i] = (BYTE)(style->colors[i] >> 24);
                        style->colors[i] &= 0xffffff;
                    }
                if(sver >= 5)	style->fontScaleX = max(style->fontScaleX, 0);
                if(sver >= 5)	style->fontScaleY = max(style->fontScaleY, 0);
#ifndef _VSMOD // patch f002. negative fontspacing at style
                if(sver >= 5)	style->fontSpacing = max(style->fontSpacing, 0);
#endif
                style->fontAngleX = style->fontAngleY = 0;
                style->borderStyle = style->borderStyle == 1 ? 0 : style->borderStyle == 3 ? 1 : 0;
                style->outlineWidthX = max(style->outlineWidthX, 0);
                style->outlineWidthY = max(style->outlineWidthY, 0);
                style->shadowDepthX = max(style->shadowDepthX, 0);
                style->shadowDepthY = max(style->shadowDepthY, 0);
                if(sver <= 4)	style->scrAlignment = (style->scrAlignment & 4) ? ((style->scrAlignment & 3) + 6) // top
                                                          : (style->scrAlignment & 8) ? ((style->scrAlignment & 3) + 3) // mid
                                                          : (style->scrAlignment & 3); // bottom

#if defined(_VSMOD)
                //set initial value for gradient
                for (int i = 0; i < 4; ++i)
                {
                    style->mod_grad.alphas[i] = style->alpha[i];
                    style->mod_grad.colors[i] = style->colors[i];

                    COLORREF revClr = revcolor(style->colors[i]);
                    for (int j = 0; j < 4; ++j)
                    {
                        style->mod_grad.alpha[i][j] = style->alpha[i];
                        style->mod_grad.color[i][j] = revClr;
                    }
                }
#endif

                StyleName.TrimLeft('*');

                ret.AddStyle(StyleName, style);
            }
            catch(...)
            {
                delete style;
                return(false);
            }
        }
        else if(entry == L"[events]")
        {
            fRet = true;
        }
        else if(entry == _T("dialogue"))
        {
            try
            {
                int hh1, mm1, ss1, ms1_div10, hh2, mm2, ss2, ms2_div10, layer = 0;
                CString Style, Actor, Effect;
                CRect marginRect;

                if(version <= 4)
                {
                    GetStr(buff, '=');    /* Marked = */
                    GetInt(buff);
                }
                if(version >= 5)layer = GetInt(buff);
                hh1 = GetInt(buff, ':');
                mm1 = GetInt(buff, ':');
                ss1 = GetInt(buff, '.');
                ms1_div10 = GetInt(buff);
                hh2 = GetInt(buff, ':');
                mm2 = GetInt(buff, ':');
                ss2 = GetInt(buff, '.');
                ms2_div10 = GetInt(buff);
                Style = WToT(GetStr(buff));
                Actor = WToT(GetStr(buff));
                marginRect.left = GetInt(buff);
                marginRect.right = GetInt(buff);
                marginRect.top = marginRect.bottom = GetInt(buff);
                if(version >= 6)marginRect.bottom = GetInt(buff);
                Effect = WToT(GetStr(buff));

                int len = min(Effect.GetLength(), buff.GetLength());
                if(Effect.Left(len) == WToT(buff.Left(len))) Effect.Empty();

                Style.TrimLeft('*');
                if(!Style.CompareNoCase(_T("Default"))) Style = _T("Default");

                ret.Add(buff,
                        file->IsUnicode(),
                        (((hh1 * 60 + mm1) * 60) + ss1) * 1000 + ms1_div10 * 10,
                        (((hh2 * 60 + mm2) * 60) + ss2) * 1000 + ms2_div10 * 10,
                        Style, Actor, Effect,
                        marginRect,
                        layer);
            }
            catch(...)
            {
                return(false);
            }
        }
        else if(entry == L"fontname")
        {
            LoadUUEFont(file);
        }
#ifdef _VSMOD // load png graphic from text resources
        else if(entry == L"filename")
        {
            ret.LoadUUEFile(file, GetStr(buff));
        }
        else if (entry == L"update details")
        {
            std::vector<TCHAR> tmp(MAX_PATH);
            size_t buflen = GetModuleFileName(0, tmp.data(), MAX_PATH);
            CString tmpstr = tmp.data();
            tmpstr.MakeLower();
            int lastSlash = tmpstr.ReverseFind(TEXT('\\'));
            if (lastSlash > 0)
                tmpstr = tmpstr.Mid(lastSlash);
            if (tmpstr.Find(TEXT("aegisub")) >= 0)
            {
                CString resPath = GetStr(buff).Trim();
                if (resPath.GetLength() > 0 && resPath[resPath.GetLength() - 1] != TEXT('\\'))
                    resPath += '\\';
                ret.m_resPath = resPath;
            }
        }
#endif
    }

    return(fRet);
}

typedef bool (*STSOpenFunct)(CTextFile* file, CSimpleTextSubtitle& ret, int CharSet);

typedef struct
{
    STSOpenFunct open;
} OpenFunctStruct;

static OpenFunctStruct OpenFuncts[] =
{
    OpenSubRipper,
    OpenSubStationAlpha,
};

static int nOpenFuncts = countof(OpenFuncts);

//

CSimpleTextSubtitle::CSimpleTextSubtitle()
{
    m_dstScreenSize = CSize(0, 0);
    m_defaultWrapStyle = 0;
    m_collisions = 0;
    m_fScaledBAS = false;
    m_encoding = CTextFile::ASCII;
    m_lcid = 0;
    m_ePARCompensationType = EPCTDisabled;
    m_dPARCompensation = 1.0;

#ifdef _VSMOD // indexing
#ifdef INDEXING
    ind_size = 0;
#endif
#endif
}

CSimpleTextSubtitle::~CSimpleTextSubtitle()
{
    Empty();
}
/*
CSimpleTextSubtitle::CSimpleTextSubtitle(CSimpleTextSubtitle& sts)
{
    *this = sts;
}

CSimpleTextSubtitle& CSimpleTextSubtitle::operator = (CSimpleTextSubtitle& sts)
{
    Empty();

    m_name = sts.m_name;
    m_dstScreenSize = sts.m_dstScreenSize;
    m_defaultWrapStyle = sts.m_defaultWrapStyle;
    m_collisions = sts.m_collisions;
    m_fScaledBAS = sts.m_fScaledBAS;
    m_fSSA = sts.m_fSSA;
    m_fUsingAutoGeneratedDefaultStyle = sts.m_fUsingAutoGeneratedDefaultStyle;
    CopyStyles(sts.m_styles);
    m_segments.Copy(sts.m_segments);
    Copy(sts);

    return(*this);
}
*/

void CSimpleTextSubtitle::Copy(CSimpleTextSubtitle& sts)
{
    Empty();

    m_name = sts.m_name;
    m_path = sts.m_path;
    m_dstScreenSize = sts.m_dstScreenSize;
    m_defaultWrapStyle = sts.m_defaultWrapStyle;
    m_collisions = sts.m_collisions;
    m_fScaledBAS = sts.m_fScaledBAS;
    m_encoding = sts.m_encoding;
    m_fUsingAutoGeneratedDefaultStyle = sts.m_fUsingAutoGeneratedDefaultStyle;
    CopyStyles(sts.m_styles);
    m_segments.Copy(sts.m_segments);
    __super::Copy(sts);
}

void CSimpleTextSubtitle::Append(CSimpleTextSubtitle& sts, int timeoff)
{
    if(timeoff < 0)
    {
        timeoff = GetCount() > 0 ? GetAt(GetCount() - 1).end : 0;
    }

    for(ptrdiff_t i = 0, j = GetCount(); i < j; i++)
    {
        if(GetAt(i).start > timeoff)
        {
            RemoveAt(i, j - i);
            break;
        }
    }

    CopyStyles(sts.m_styles, true);

    for(ptrdiff_t i = 0, j = sts.GetCount(); i < j; i++)
    {
        STSEntry stse = sts.GetAt(i);
        stse.start += timeoff;
        stse.end += timeoff;
        stse.readorder += GetCount();
        __super::Add(stse);
    }

    CreateSegments();
}

void CSTSStyleMap::Free()
{
    POSITION pos = GetStartPosition();
    while(pos)
    {
        CString key;
        STSStyle* val;
        GetNextAssoc(pos, key, val);
        delete val;
    }

    RemoveAll();
}

bool CSimpleTextSubtitle::CopyStyles(const CSTSStyleMap& styles, bool fAppend)
{
    if(!fAppend) m_styles.Free();

    POSITION pos = styles.GetStartPosition();
    while(pos)
    {
        CString key;
        STSStyle* val;
        styles.GetNextAssoc(pos, key, val);

        STSStyle* s = new STSStyle;
        if(!s) return(false);

        *s = *val;

        AddStyle(key, s);
    }

    return(true);
}

void CSimpleTextSubtitle::Empty()
{
    m_dstScreenSize = CSize(0, 0);
    m_styles.Free();
    m_segments.RemoveAll();
    RemoveAll();

#ifdef _VSMOD // indexing
#ifdef INDEXING
    if(ind_size>0)
    {
        delete ind_time;
        delete ind_pos;
    }
#endif
#endif
}

void CSimpleTextSubtitle::Add(CStringW str, bool fUnicode, int start, int end, CString style, CString actor, CString effect, CRect marginRect, int layer, int readorder)
{
    if(str.Trim().IsEmpty() || start > end) return;

    str.Remove('\r');
    str.Replace(L"\n", L"\\N");
    if(style.IsEmpty()) style = _T("Default");
    style.TrimLeft('*');

    STSEntry sub;
    sub.str = str;
    sub.fUnicode = fUnicode;
    sub.style = style;
    sub.actor = actor;
    sub.effect = effect;
    sub.marginRect = marginRect;
    sub.layer = layer;
    sub.start = start;
    sub.end = end;
    sub.readorder = readorder < 0 ? GetCount() : readorder;

    int n = __super::Add(sub);
}

#ifdef _VSMOD
#ifdef INDEXING
void CSimpleTextSubtitle::MakeIndex(int SizeOfSegment)
{
    int cnt = m_segments.GetCount();
    if (SizeOfSegment==0) // autosize
    {
        // 100000 lines == 1300 segments
        // TODO: make gooood =D
        if(cnt<100)
        {
            SizeOfSegment = (cnt==0) ? 1 : cnt;
        }
        else if (cnt<1000)
        {
            SizeOfSegment = cnt / 50;
        }
        else
        {
            SizeOfSegment = cnt / 100;
        }
    }

    ind_size = cnt / SizeOfSegment;
    
    ind_time = new DWORD[ind_size];
    ind_pos = new DWORD[ind_size];

    for(auto i = 0; i<ind_size; i++)
    {
        auto pos = i * SizeOfSegment;
        ind_time[i] = m_segments[pos].start;
        ind_pos[i] = pos;
    }
}
#endif
#endif

STSStyle* CSimpleTextSubtitle::CreateDefaultStyle(int CharSet)
{
    CString def(_T("Default"));

    STSStyle* ret = NULL;

    if(!m_styles.Lookup(def, ret))
    {
        STSStyle* style = new STSStyle();
        style->charSet = CharSet;
        AddStyle(def, style);
        m_styles.Lookup(def, ret);

        m_fUsingAutoGeneratedDefaultStyle = true;
    }
    else
    {
        m_fUsingAutoGeneratedDefaultStyle = false;
    }

    return ret;
}

void CSimpleTextSubtitle::ChangeUnknownStylesToDefault()
{
    CAtlMap<CString, STSStyle*, CStringElementTraits<CString> > unknown;
    bool fReport = true;

    for(ptrdiff_t i = 0; i < GetCount(); i++)
    {
        STSEntry& stse = GetAt(i);

        STSStyle* val;
        if(!m_styles.Lookup(stse.style, val))
        {
            if(!unknown.Lookup(stse.style, val))
            {
                if(fReport)
                {
                    CString msg;
                    msg.Format(_T("Unknown style found: \"%s\", changed to \"Default\"!\n\nPress Cancel to ignore further warnings."), stse.style);
                    if(MessageBox(NULL, msg, _T("Warning"), MB_OKCANCEL | MB_ICONWARNING) != IDOK) fReport = false;
                }

                unknown[stse.style] = NULL;
            }

            stse.style = _T("Default");
        }
    }
}

void CSimpleTextSubtitle::AddStyle(CString name, STSStyle* style)
{
    int i, j;

    if(name.IsEmpty()) name = _T("Default");

    STSStyle* val;
    if(m_styles.Lookup(name, val))
    {
        if(*val == *style)
        {
            delete style;
            return;
        }

        int len = name.GetLength();

        for(i = len; i > 0 && _istdigit(name[i-1]); i--);

        int idx = 1;

        CString name2 = name;

        if(i < len && _stscanf(name.Right(len - i), _T("%d"), &idx) == 1)
        {
            name2 = name.Left(i);
        }

        idx++;

        CString name3;
        do
        {
            name3.Format(_T("%s%d"), name2, idx);
            idx++;
        }
        while(m_styles.Lookup(name3));

        m_styles.RemoveKey(name);
        m_styles[name3] = val;

        for(i = 0, j = GetCount(); i < j; i++)
        {
            STSEntry& stse = GetAt(i);
            if(stse.style == name) stse.style = name3;
        }
    }

    m_styles[name] = style;
}

bool CSimpleTextSubtitle::SetDefaultStyle(STSStyle& s)
{
    STSStyle* val;
    if(!m_styles.Lookup(_T("Default"), val)) return false;
    *val = s;
    m_fUsingAutoGeneratedDefaultStyle = false;
    return true;
}

bool CSimpleTextSubtitle::GetDefaultStyle(STSStyle& s)
{
    STSStyle* val;
    if(!m_styles.Lookup(_T("Default"), val)) return false;
    s = *val;
    return true;
}

int CSimpleTextSubtitle::SearchSub(int t)
{
    int i = 0, j = GetCount() - 1, ret = -1;

    if(j >= 0 && t >= TranslateStart(j))
    {
        return(j);
    }

    while(i < j)
    {
        int mid = (i + j) >> 1;

        int midt = TranslateStart(mid);

        if(t == midt)
        {
            while(mid > 0 && t == TranslateStart(mid - 1)) mid--;
            ret = mid;
            break;
        }
        else if(t < midt)
        {
            ret = -1;
            if(j == mid) mid--;
            j = mid;
        }
        else if(t > midt)
        {
            ret = mid;
            if(i == mid) mid++;
            i = mid;
        }
    }

    return(ret);
}

const STSSegment* CSimpleTextSubtitle::SearchSubs(int t, /*[out]*/ int* iSegment, int* nSegments)
{
    int i = 0, j = m_segments.GetCount() - 1, ret = -1;

    if(nSegments) *nSegments = j + 1;

    // last segment
    if(j >= 0 && t >= TranslateSegmentStart(j) && t < TranslateSegmentEnd(j))
    {
        if(iSegment) *iSegment = j;
        return(&m_segments[j]);
    }

    // after last segment
    if(j >= 0 && t >= TranslateSegmentEnd(j))
    {
        if(iSegment) *iSegment = j + 1;
        return(NULL);
    }

    // before first segment
    if(j > 0 && t < TranslateSegmentStart(i))
    {
        if(iSegment) *iSegment = -1;
        return(NULL);
    }

#ifdef _VSMOD
#ifdef INDEXING
    // find bounds
    // is this nya?
    for(ptrdiff_t k = 0; k < ind_size; k++)
    {
        if(ind_time[k]>t)
        {
            if(k==0) break;
            i = ind_pos[k-1];
            j = ind_pos[k];
            break;
        }
    }
#endif
#endif

    while(i < j)
    {
        int mid = (i + j) >> 1;

        int midt = TranslateSegmentStart(mid);

        if(t == midt)
        {
            ret = mid;
            break;
        }
        else if(t < midt)
        {
            ret = -1;
            if(j == mid) mid--;
            j = mid;
        }
        else if(t > midt)
        {
            ret = mid;
            if(i == mid) mid++;
            i = mid;
        }
    }

    if(0 <= ret && ret < m_segments.GetCount())
    {
        if(iSegment) *iSegment = ret;
    }

    if(0 <= ret && ret < m_segments.GetCount()
       && m_segments[ret].subs.GetCount() > 0
       && TranslateSegmentStart(ret) <= t && t < TranslateSegmentEnd(ret))
    {
        return(&m_segments[ret]);
    }

    return(NULL);
}

int CSimpleTextSubtitle::TranslateStart(int i)
{
    return(i < 0 || GetCount() <= i ? -1 : GetAt(i).start);
}

int CSimpleTextSubtitle::TranslateEnd(int i)
{
    return(i < 0 || GetCount() <= i ? -1 : GetAt(i).end);
}

int CSimpleTextSubtitle::TranslateSegmentStart(int i)
{
    return(i < 0 || m_segments.GetCount() <= i ? -1 : m_segments[i].start);
}

int CSimpleTextSubtitle::TranslateSegmentEnd(int i)
{
    return(i < 0 || m_segments.GetCount() <= i ? -1 : m_segments[i].end);
}

STSStyle* CSimpleTextSubtitle::GetStyle(int i)
{
    CString def = _T("Default");

    STSStyle* style = NULL;
    m_styles.Lookup(GetAt(i).style, style);

    STSStyle* defstyle = NULL;
    m_styles.Lookup(def, defstyle);

    if(!style)
    {
        style = defstyle;
    }

    ASSERT(style);

    return style;
}

bool CSimpleTextSubtitle::GetStyle(int i, STSStyle& stss)
{
    CString def = _T("Default");

    STSStyle* style = NULL;
    m_styles.Lookup(GetAt(i).style, style);

    STSStyle* defstyle = NULL;
    m_styles.Lookup(def, defstyle);

    if(!style)
    {
        if(!defstyle)
        {
            defstyle = CreateDefaultStyle(DEFAULT_CHARSET);
        }

        style = defstyle;
    }

    if(!style)
    {
        ASSERT(0);
        return false;
    }

    stss = *style;
    if(stss.relativeTo == 2 && defstyle)
        stss.relativeTo = defstyle->relativeTo;

    return true;
}

int CSimpleTextSubtitle::GetCharSet(int i)
{
    STSStyle stss;
    GetStyle(i, stss);
    return(stss.charSet);
}

bool CSimpleTextSubtitle::IsEntryUnicode(int i)
{
    return(GetAt(i).fUnicode);
}

void CSimpleTextSubtitle::ConvertUnicode(int i, bool fUnicode)
{
    STSEntry& stse = GetAt(i);

    if(stse.fUnicode ^ fUnicode)
    {
        int CharSet = GetCharSet(i);

        stse.str = fUnicode
                   ? MBCSSSAToUnicode(stse.str, CharSet)
                   : UnicodeSSAToMBCS(stse.str, CharSet);

        stse.fUnicode = fUnicode;
    }
}

CStringA CSimpleTextSubtitle::GetStrA(int i, bool fSSA)
{
    return(WToA(GetStrWA(i, fSSA)));
}

CStringW CSimpleTextSubtitle::GetStrW(int i, bool fSSA)
{
    bool fUnicode = IsEntryUnicode(i);
    int CharSet = GetCharSet(i);

    CStringW str = GetAt(i).str;

    if(!fUnicode)
        str = MBCSSSAToUnicode(str, CharSet);

    if(!fSSA)
        str = RemoveSSATags(str, fUnicode, CharSet);

    return(str);
}

CStringW CSimpleTextSubtitle::GetStrWA(int i, bool fSSA)
{
    bool fUnicode = IsEntryUnicode(i);
    int CharSet = GetCharSet(i);

    CStringW str = GetAt(i).str;

    if(fUnicode)
        str = UnicodeSSAToMBCS(str, CharSet);

    if(!fSSA)
        str = RemoveSSATags(str, fUnicode, CharSet);

    return(str);
}

void CSimpleTextSubtitle::SetStr(int i, CStringA str, bool fUnicode)
{
    SetStr(i, AToW(str), false);
}

void CSimpleTextSubtitle::SetStr(int i, CStringW str, bool fUnicode)
{
    STSEntry& stse = GetAt(i);

    str.Replace(L"\n", L"\\N");

    if(stse.fUnicode && !fUnicode) stse.str = MBCSSSAToUnicode(str, GetCharSet(i));
    else if(!stse.fUnicode && fUnicode) stse.str = UnicodeSSAToMBCS(str, GetCharSet(i));
    else stse.str = str;
}

static int comp1(const void* a, const void* b)
{
    int ret = ((STSEntry*)a)->start - ((STSEntry*)b)->start;
    if(ret == 0) ret = ((STSEntry*)a)->layer - ((STSEntry*)b)->layer;
    if(ret == 0) ret = ((STSEntry*)a)->readorder - ((STSEntry*)b)->readorder;
    return(ret);
}

static int comp2(const void* a, const void* b)
{
    return(((STSEntry*)a)->readorder - ((STSEntry*)b)->readorder);
}

void CSimpleTextSubtitle::Sort(bool fRestoreReadorder)
{
    qsort(GetData(), GetCount(), sizeof(STSEntry), !fRestoreReadorder ? comp1 : comp2);
    CreateSegments();
}

static int intcomp(const void* i1, const void* i2)
{
    return(*((int*)i1) - *((int*)i2));
}

void CSimpleTextSubtitle::CreateSegments()
{
    m_segments.RemoveAll();

    int i, j;

    CAtlArray<int> breakpoints;

    for(i = 0; i < GetCount(); i++)
    {
        STSEntry& stse = GetAt(i);
        breakpoints.Add(stse.start);
        breakpoints.Add(stse.end);
    }

    qsort(breakpoints.GetData(), breakpoints.GetCount(), sizeof(int), intcomp);

    int* ptr = breakpoints.GetData(), prev = ptr ? *ptr : NULL;

    for(i = breakpoints.GetCount(); i > 0; i--, ptr++)
    {
        if(*ptr != prev)
        {
            m_segments.Add(STSSegment(prev, *ptr));
            prev = *ptr;
        }
    }

    for(i = 0; i < GetCount(); i++)
    {
        STSEntry& stse = GetAt(i);
        for(j = 0; j < m_segments.GetCount() && m_segments[j].start < stse.start; j++);
        for(; j < m_segments.GetCount() && m_segments[j].end <= stse.end; j++)
            m_segments[j].subs.Add(i);
    }

    OnChanged();
    /*
    for(i = 0, j = m_segments.GetCount(); i < j; i++)
    {
        STSSegment& stss = m_segments[i];

        TRACE(_T("%d - %d"), stss.start, stss.end);

        for(ptrdiff_t k = 0, l = stss.subs.GetCount(); k < l; k++)
        {
            TRACE(_T(", %d"), stss.subs[k]);
        }

        TRACE(_T("\n"));
    }
    */
}

bool CSimpleTextSubtitle::Open(CString fn, int CharSet, CString name)
{
    Empty();

    CTextFile f;
    if(!f.Open(fn)) return(false);

    fn.Replace('\\', '/');
    if(name.IsEmpty())
    {
        name = fn.Left(fn.ReverseFind('.'));
        name = name.Mid(name.ReverseFind('/') + 1);
        int len = name.GetLength();
        int pos = name.ReverseFind('.') + 1;
        if ((len - pos) > 1)
            name = name.Mid(pos);
    }

    return(Open(&f, CharSet, name));
}

static int CountLines(CTextFile* f, ULONGLONG from, ULONGLONG to)
{
    int n = 0;
    CString s;
    f->Seek(from, 0);
    while(f->ReadString(s) && f->GetPosition() < to) n++;
    return(n);
}

bool CSimpleTextSubtitle::Open(CTextFile* f, int CharSet, CString name)
{
    Empty();

    ULONGLONG pos = f->GetPosition();

    for(ptrdiff_t i = 0; i < nOpenFuncts; i++)
    {
        if(!OpenFuncts[i].open(f, *this, CharSet) /*|| !GetCount()*/)
        {
            if(GetCount() > 0)
            {
                int n = CountLines(f, pos, f->GetPosition());
                CString s;
                s.Format(_T("Syntax error at line %d!\t"), n + 1);
                AfxMessageBox(s, MB_OK | MB_ICONERROR);
                Empty();
                break;
            }

            f->Seek(pos, 0);
            Empty();
            continue;
        }

        m_name = name;
        m_encoding = f->GetEncoding();
        m_path = f->GetFilePath();

//		Sort();
        CreateSegments();
#ifdef _VSMOD // indexing
#ifdef INDEXING
        MakeIndex(0);
#endif
#endif
        CTextFile f2;
        if(f2.Open(f->GetFilePath() + _T(".style")))
            OpenSubStationAlpha(&f2, *this, CharSet);

        CreateDefaultStyle(CharSet);

        ChangeUnknownStylesToDefault();

        if(m_dstScreenSize == CSize(0, 0)) m_dstScreenSize = CSize(384, 288);

        return(true);
    }

    return(false);
}

bool CSimpleTextSubtitle::Open(BYTE* data, int len, int CharSet, CString name)
{
    TCHAR path[_MAX_PATH];
    if(!GetTempPath(_MAX_PATH, path)) return(false);

    TCHAR fn[_MAX_PATH];
    if(!GetTempFileName(path, _T("vs"), 0, fn)) return(false);

    FILE* tmp = _tfopen(fn, _T("wb"));
    if(!tmp) return(false);

    int i = 0;
    for(; i <= (len - 1024); i += 1024) fwrite(&data[i], 1024, 1, tmp);
    if(len > i) fwrite(&data[i], len - i, 1, tmp);

    fclose(tmp);

    bool fRet = Open(fn, CharSet, name);

    _tremove(fn);

    return(fRet);
}

////////////////////////////////////////////////////////////////////

STSStyle::STSStyle()
{
    SetDefault();
}

#ifdef _VSMOD
STSStyle::STSStyle(STSStyle& s)
{
    SetDefault();
    mod_CopyStyleFrom(s);
}
#endif

void STSStyle::SetDefault()
{
    marginRect = CRect(20, 20, 20, 20);
    scrAlignment = 2;
    borderStyle = 0;
    outlineWidthX = outlineWidthY = 2;
    shadowDepthX = shadowDepthY = 3;
    colors[0] = 0x00ffffff;
    colors[1] = 0x0000ffff;
    colors[2] = 0x00000000;
    colors[3] = 0x00000000;
    alpha[0] = 0x00;
    alpha[1] = 0x00;
    alpha[2] = 0x00;
    alpha[3] = 0x80;
    charSet = DEFAULT_CHARSET;
    fontName = _T("Arial");
    fontSize = 18;
    fontScaleX = fontScaleY = 100;
    fontSpacing = 0;
    fontWeight = FW_BOLD;
    fItalic = false;
    fUnderline = false;
    fStrikeOut = false;
    fBlur = 0;
    fGaussianBlur = 0;
    fontShiftX = fontShiftY = fontAngleZ = fontAngleX = fontAngleY = 0;
    relativeTo = 2;
#ifdef _VSMOD
    // patch m001. Vertical fontspacing
    mod_verticalSpace = 0;
    // patch m002. Z-coord
    mod_z = 0;
    // patch m003. random text points
    mod_rand.clear();
    // patch m004. gradient colors
    mod_grad.clear();
    // patch m007. symbol rotating
    mod_fontOrient = 0;
#endif
}

bool STSStyle::operator == (STSStyle& s)
{
    return(marginRect == s.marginRect
           && scrAlignment == s.scrAlignment
           && borderStyle == s.borderStyle
           && outlineWidthX == s.outlineWidthX
           && outlineWidthY == s.outlineWidthY
           && shadowDepthX == s.shadowDepthX
           && shadowDepthY == s.shadowDepthY
           && *((int*)&colors[0]) == *((int*)&s.colors[0])
           && *((int*)&colors[1]) == *((int*)&s.colors[1])
           && *((int*)&colors[2]) == *((int*)&s.colors[2])
           && *((int*)&colors[3]) == *((int*)&s.colors[3])
           && alpha[0] == s.alpha[0]
           && alpha[1] == s.alpha[1]
           && alpha[2] == s.alpha[2]
           && alpha[3] == s.alpha[3]
           && fBlur == s.fBlur
           && fGaussianBlur == s.fGaussianBlur
           && relativeTo == s.relativeTo
#ifdef _VSMOD
           // patch m001. Vertical fontspacing
           && mod_verticalSpace == s.mod_verticalSpace
           // patch m002. Z-coord
           && mod_z == s.mod_z
           // patch m003. random text points
           && mod_rand == s.mod_rand
           // patch m004. gradient colors
           && mod_grad == s.mod_grad
           // patch m007. symbol rotating
           && mod_fontOrient == s.mod_fontOrient
           // patch m008. distort
           && mod_distort == s.mod_distort
           // patch m011. jitter
           && mod_jitter == s.mod_jitter
#endif
           && IsFontStyleEqual(s));
}

bool STSStyle::IsFontStyleEqual(STSStyle& s)
{
    return(
              charSet == s.charSet
              && fontName == s.fontName
              && fontSize == s.fontSize
              && fontScaleX == s.fontScaleX
              && fontScaleY == s.fontScaleY
              && fontSpacing == s.fontSpacing
              && fontWeight == s.fontWeight
              && fItalic == s.fItalic
              && fUnderline == s.fUnderline
              && fStrikeOut == s.fStrikeOut
              && fontAngleZ == s.fontAngleZ
              && fontAngleX == s.fontAngleX
              && fontAngleY == s.fontAngleY
              // patch f001. fax fay patch (many instances at line)
              && fontShiftX == s.fontShiftX
              && fontShiftY == s.fontShiftY);
}

#ifdef _VSMOD
void STSStyle::mod_CopyStyleFrom(STSStyle& s)
{
    marginRect = s.marginRect;
    scrAlignment = s.scrAlignment;
    borderStyle = s.borderStyle;
    outlineWidthX = s.outlineWidthX;
    outlineWidthY = s.outlineWidthY;
    shadowDepthX = s.shadowDepthX;
    shadowDepthY = s.shadowDepthY;
    *((int*)&colors[0]) = *((int*)&s.colors[0]);
    *((int*)&colors[1]) = *((int*)&s.colors[1]);
    *((int*)&colors[2]) = *((int*)&s.colors[2]);
    *((int*)&colors[3]) = *((int*)&s.colors[3]);
    alpha[0] = s.alpha[0];
    alpha[1] = s.alpha[1];
    alpha[2] = s.alpha[2];
    alpha[3] = s.alpha[3];
    fBlur = s.fBlur;
    fGaussianBlur = s.fGaussianBlur;
    relativeTo = s.relativeTo;

    //patch m001. Vertical fontspacing
    mod_verticalSpace = s.mod_verticalSpace;
    //patch m002. Z-coord
    mod_z = s.mod_z;
    //patch m003. random text points
    mod_rand = s.mod_rand;
    //patch m004. gradient colors
    mod_grad = s.mod_grad;
    // patch m007. symbol rotating
    mod_fontOrient = s.mod_fontOrient;
    // patch m008. distort
    mod_distort = s.mod_distort;
    // patch m011. jitter
    mod_jitter = s.mod_jitter;
    // font
    charSet = s.charSet;
    fontName = s.fontName;
    fontSize = s.fontSize;
    fontScaleX = s.fontScaleX;
    fontScaleY = s.fontScaleY;
    fontSpacing = s.fontSpacing;
    fontWeight = s.fontWeight;
    fItalic = s.fItalic;
    fUnderline = s.fUnderline;
    fStrikeOut = s.fStrikeOut;
    fontAngleZ = s.fontAngleZ;
    fontAngleX = s.fontAngleX;
    fontAngleY = s.fontAngleY;
    // patch f001. fax fay patch (many instances at line)
    fontShiftX = s.fontShiftX;
    fontShiftY = s.fontShiftY;
}

void STSStyle::operator = (STSStyle& s)
{
    mod_CopyStyleFrom(s);
}
#endif

void STSStyle::operator = (LOGFONT& lf)
{
    charSet = lf.lfCharSet;
    fontName = lf.lfFaceName;
    fontSize = -MulDiv(lf.lfHeight, 72, 96);
//	fontAngleZ = (float)(1.0*lf.lfEscapement/10);
    fontWeight = lf.lfWeight;
    fItalic = !!lf.lfItalic;
    fUnderline = !!lf.lfUnderline;
    fStrikeOut = !!lf.lfStrikeOut;
}

LOGFONTA& operator <<= (LOGFONTA& lfa, STSStyle& s)
{
    lfa.lfCharSet = s.charSet;
    strncpy_s(lfa.lfFaceName, LF_FACESIZE, CStringA(s.fontName), _TRUNCATE);
    lfa.lfHeight = -MulDiv((int)(s.fontSize + 0.5), 96, 72);
    lfa.lfWeight = s.fontWeight;
    lfa.lfItalic = s.fItalic ? -1 : 0;
    lfa.lfUnderline = s.fUnderline ? -1 : 0;
    lfa.lfStrikeOut = s.fStrikeOut ? -1 : 0;
    return(lfa);
}

LOGFONTW& operator <<= (LOGFONTW& lfw, STSStyle& s)
{
    lfw.lfCharSet = s.charSet;
    wcsncpy_s(lfw.lfFaceName, LF_FACESIZE, CStringW(s.fontName), _TRUNCATE);
    lfw.lfHeight = -MulDiv((int)(s.fontSize + 0.5), 96, 72);
    lfw.lfWeight = s.fontWeight;
    lfw.lfItalic = s.fItalic ? -1 : 0;
    lfw.lfUnderline = s.fUnderline ? -1 : 0;
    lfw.lfStrikeOut = s.fStrikeOut ? -1 : 0;
    return(lfw);
}

CString& operator <<= (CString& style, STSStyle& s)
{
    style.Format(_T("%d;%d;%d;%d;%d;%d;%f;%f;%f;%f;0x%06x;0x%06x;0x%06x;0x%06x;0x%02x;0x%02x;0x%02x;0x%02x;%d;%s;%f;%f;%f;%f;%d;%d;%d;%d;%d;%f;%f;%f;%f;%d"),
                 s.marginRect.left, s.marginRect.right, s.marginRect.top, s.marginRect.bottom,
                 s.scrAlignment, s.borderStyle,
                 s.outlineWidthX, s.outlineWidthY, s.shadowDepthX, s.shadowDepthY,
                 s.colors[0], s.colors[1], s.colors[2], s.colors[3],
                 s.alpha[0], s.alpha[1], s.alpha[2], s.alpha[3],
                 s.charSet,
                 s.fontName, s.fontSize,
                 s.fontScaleX, s.fontScaleY,
                 s.fontSpacing, s.fontWeight,
                 (int)s.fItalic, (int)s.fUnderline, (int)s.fStrikeOut, s.fBlur, s.fGaussianBlur,
                 s.fontAngleZ, s.fontAngleX, s.fontAngleY,
                 s.relativeTo);

    return(style);
}

STSStyle& operator <<= (STSStyle& s, CString& style)
{
    s.SetDefault();

    try
    {
        CStringW str = TToW(style);
        if(str.Find(';') >= 0)
        {
            s.marginRect.left = GetInt(str, ';');
            s.marginRect.right = GetInt(str, ';');
            s.marginRect.top = GetInt(str, ';');
            s.marginRect.bottom = GetInt(str, ';');
            s.scrAlignment = GetInt(str, ';');
            s.borderStyle = GetInt(str, ';');
            s.outlineWidthX = GetFloat(str, ';');
            s.outlineWidthY = GetFloat(str, ';');
            s.shadowDepthX = GetFloat(str, ';');
            s.shadowDepthY = GetFloat(str, ';');
            for(ptrdiff_t i = 0; i < 4; i++) s.colors[i] = (COLORREF)GetInt(str, ';');
            for(ptrdiff_t i = 0; i < 4; i++) s.alpha[i] = GetInt(str, ';');
            s.charSet = GetInt(str, ';');
            s.fontName = WToT(GetStr(str, ';'));
            s.fontSize = GetFloat(str, ';');
            s.fontScaleX = GetFloat(str, ';');
            s.fontScaleY = GetFloat(str, ';');
            s.fontSpacing = GetFloat(str, ';');
            s.fontWeight = GetInt(str, ';');
            s.fItalic = !!GetInt(str, ';');
            s.fUnderline = !!GetInt(str, ';');
            s.fStrikeOut = !!GetInt(str, ';');
            s.fBlur = GetInt(str, ';');
            s.fGaussianBlur = GetFloat(str, ';');
            s.fontAngleZ = GetFloat(str, ';');
            s.fontAngleX = GetFloat(str, ';');
            s.fontAngleY = GetFloat(str, ';');
            s.relativeTo = GetInt(str, ';');
        }
    }
    catch(...)
    {
        s.SetDefault();
    }

    return(s);
}

#ifdef _VSMOD // patch m003. random text points
bool MOD_RANDOM::operator == (MOD_RANDOM& mr)
{
    return (X == mr.X
            && Y == mr.Y
            && Z == mr.X
            && Seed == mr.Seed);
}

void MOD_RANDOM::clear()
{
    X = 0;
    Y = 0;
    Z = 0;
    Seed = 0;
}
#endif

#ifdef _VSMOD // patch m004. gradient colors
#include <png.h> // patch m010. png background

MOD_PNGIMAGE::MOD_PNGIMAGE()
{
    width = 0;
    height = 0;

    xoffset = 0;
    yoffset = 0;

    pointer = NULL;

    // rasterizer
    alpha = 0xFF;
}

// read embedded graphics
void png_default_read_edata(png_structp png_ptr, png_bytep data, png_size_t length)
{
    png_size_t check;

    if(png_ptr->io_ptr == NULL)
        return;

    BYTE* eldata = (BYTE*)png_ptr->io_ptr;

    // read from memory
    memcpy(data, eldata, length);
    eldata += length;
    png_ptr->io_ptr = (png_voidp)eldata;
}

bool MOD_PNGIMAGE::operator == (MOD_PNGIMAGE& png)
{
    return(filename == png.filename
           && xoffset == png.xoffset
           && yoffset == png.yoffset);
}

bool MOD_PNGIMAGE::processData(png_structp png_ptr)
{
    png_uint_32 color_type;
    png_uint_32 bit_depth;

    png_infop info_ptr;
    int number_of_passes;

    /* initialize stuff */
    info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr) return false;  // png_create_info_struct failed

    if(setjmp(png_jmpbuf(png_ptr))) return false;  // Error during init_io

    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    width = info_ptr->width;
    height = info_ptr->height;
    color_type = info_ptr->color_type;
    bit_depth = info_ptr->bit_depth;

    // palette
    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);

    // expand to 8 bits
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);

    if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);

    // Strip 16 bit depth files to 8 bit depth
    if(bit_depth == 16)
        png_set_strip_16(png_ptr);

    // ARGB -> RGBA
//	if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
//        png_set_swap_alpha(png_ptr);

    // grayscale -> RGB
    if(color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_ptr);

    number_of_passes = png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);

    /* read file */
    if(setjmp(png_jmpbuf(png_ptr))) return false;  // Error during read_image

    bpp = info_ptr->rowbytes / width;
    pointer = (png_bytep*) malloc(sizeof(png_bytep) * height);
    for(int y = 0; y < height; y++)
        pointer[y] = (png_byte*) malloc(info_ptr->rowbytes);

    png_read_image(png_ptr, pointer);
    return true;
}

bool MOD_PNGIMAGE::initImage(CString m_fn)
{
    if((m_fn == filename) && (pointer != NULL)) return true; // already loaded

    char header[8];	// 8 is the maximum size that can be check
    png_structp png_ptr;

    filename = m_fn;

    FILE *fp = _wfopen(CT2WEX<>(m_fn, CP_THREAD_ACP), L"rb");
    bool retVal = false;
    do
    {
        if (!fp) // File could not be opened for reading
            break;

        fread(header, 1, 8, fp);
        if (png_sig_cmp((png_bytep)header, 0, 8)) // File is not recognized as a PNG file
            break;

        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) // png_create_read_struct failed
            break;

        png_init_io(png_ptr, fp);
        retVal = processData(png_ptr);
    } while (false);
    if (fp)
        fclose(fp);
    return retVal;
}

bool MOD_PNGIMAGE::initImage(BYTE* data, CString m_fn)
{
    if((m_fn == filename) && (pointer != NULL)) return true; // already loaded
    if(data == NULL) return false; // not loaded

    char header[8];	// 8 is the maximum size that can be check
    png_structp png_ptr;

    filename = m_fn;

    memcpy(header, data, 8);
    if(png_sig_cmp((png_bytep)header, 0, 8)) return false;  // File is not recognized as a PNG file

    data += 8; // don't forget modify pointer
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr) return false;  // png_create_read_struct failed

    png_set_read_fn(png_ptr, (png_voidp)data, &png_default_read_edata);
    return processData(png_ptr);
}

void MOD_PNGIMAGE::freeImage()
{
    if(pointer != NULL) delete [] pointer;
}

MOD_GRADIENT::MOD_GRADIENT()
{
    clear();
}

bool MOD_GRADIENT::operator == (MOD_GRADIENT& mg)
{
    return (color[0][0] == mg.color[0][0] // T.T
            && color[1][0] == mg.color[1][0]
            && color[2][0] == mg.color[2][0]
            && color[3][0] == mg.color[3][0]
            && color[0][1] == mg.color[0][1]
            && color[1][1] == mg.color[1][1]
            && color[2][1] == mg.color[2][1]
            && color[3][1] == mg.color[3][1]
            && color[0][2] == mg.color[0][2]
            && color[1][2] == mg.color[1][2]
            && color[2][2] == mg.color[2][2]
            && color[3][2] == mg.color[3][2]
            && color[0][3] == mg.color[0][3]
            && color[1][3] == mg.color[1][3]
            && color[2][3] == mg.color[2][3]
            && color[3][3] == mg.color[3][3]
            && alpha[0][0] == mg.alpha[0][0]
            && alpha[1][0] == mg.alpha[1][0]
            && alpha[2][0] == mg.alpha[2][0]
            && alpha[3][0] == mg.alpha[3][0]
            && alpha[0][1] == mg.alpha[0][1]
            && alpha[1][1] == mg.alpha[1][1]
            && alpha[2][1] == mg.alpha[2][1]
            && alpha[3][1] == mg.alpha[3][1]
            && alpha[0][2] == mg.alpha[0][2]
            && alpha[1][2] == mg.alpha[1][2]
            && alpha[2][2] == mg.alpha[2][2]
            && alpha[3][2] == mg.alpha[3][2]
            && alpha[0][3] == mg.alpha[0][3]
            && alpha[1][3] == mg.alpha[1][3]
            && alpha[2][3] == mg.alpha[2][3]
            && alpha[3][3] == mg.alpha[3][3]
            && mode[0] == mg.mode[0]
            && mode[1] == mg.mode[1]
            && mode[2] == mg.mode[2]
            && mode[3] == mg.mode[3]
            && b_images[0] == mg.b_images[0]
            && b_images[1] == mg.b_images[1]
            && b_images[2] == mg.b_images[2]
            && b_images[3] == mg.b_images[3]);
}

void MOD_GRADIENT::clear()
{
    memset(&color, 0, sizeof(color));
    memset(&colors, 0, sizeof(colors));
    memset(&alpha, 0, sizeof(alpha));
    memset(&alphas, 0, sizeof(alphas));
    memset(&mode, 0, sizeof(mode));
    colors[0] = 0x00ffffff;
    colors[1] = 0x0000ffff;
    alphas[3] = 0x80;
    width = 0;
    height = 0;
    xoffset = 0;
    yoffset = 0;
    subpixx = 0;
    subpixy = 0;
    fadalpha = 0xFF;
}

#include <math.h>
DWORD MOD_GRADIENT::getmixcolor(int tx, int ty, int i) // too slow T.T
{
    DWORD colorb = 0;
    tx += xoffset;
    // gradient
    if(mode[i] == 1)
    {
        double x = (double)tx / (double)width;
        double y = (double)ty / (double)height;
        for(int j = 0; j < 3; j++)
        {
            colorb |= ((DWORD)(((color[i][0] >> (8 * j)) & 0xff) * (1 - x) * y +
                               ((color[i][1] >> (8 * j)) & 0xff) * x * y +
                               ((color[i][2] >> (8 * j)) & 0xff) * (1 - y) * (1 - x) +
                               ((color[i][3] >> (8 * j)) & 0xff) * x * (1 - y)) & 0xff) << (8 * j);
        }
        DWORD al = (DWORD)((alpha[i][0] * (1 - x) * y) +
                           (alpha[i][1] * x * y) +
                           (alpha[i][2] * (1 - y) * (1 - x)) +
                           (alpha[i][3] * x * (1 - y))) & 0xff;
        colorb  |= (((0xff - al) * (0xff - fadalpha)) & 0xff00) << (16);
        return colorb;
    }
    // png background
    if(mode[i] == 2)
    {
        // unwarp
        tx += b_images[i].xoffset;
        ty += b_images[i].yoffset;
        while(tx > b_images[i].width - 1) tx -= b_images[i].width;
        while(ty > b_images[i].height - 1) ty -= b_images[i].height;
        while(tx < 0) tx += b_images[i].width;
        while(ty < 0) ty += b_images[i].height;
        // now tx and ty are valid array indexes
        // rows are inverted last,...,n,...,1,0
        bool nlastpixx = (tx > 0);
        bool nlastpixy = (ty < b_images[i].height - 1);
        BYTE* dst11 = b_images[i].pointer[b_images[i].height - 1 - ty] + tx * b_images[i].bpp;
        BYTE* dst12 = (nlastpixx) ? b_images[i].pointer[b_images[i].height - 1 - ty] + (tx - 1) * b_images[i].bpp : NULL;
        BYTE* dst21 = (nlastpixy) ? b_images[i].pointer[b_images[i].height - ty - 2] + tx * b_images[i].bpp : NULL;
        BYTE* dst22 = (nlastpixx && nlastpixy) ? b_images[i].pointer[b_images[i].height - ty - 2] + (tx - 1) * b_images[i].bpp : NULL;
        BYTE r = dst11[0];
        BYTE g = dst11[1];
        BYTE b = dst11[2];
        BYTE a = (b_images[i].bpp == 4) ? dst11[3] : 0xFF;
        // subpixel positioning
        if(nlastpixx && !nlastpixy) // last row
        {
            r = (r * (8 - subpixx) + dst12[0] * subpixx) >> 3;
            g = (g * (8 - subpixx) + dst12[1] * subpixx) >> 3;
            b = (b * (8 - subpixx) + dst12[2] * subpixx) >> 3;
            a = (b_images[i].bpp == 4) ? (a * (8 - subpixx) + dst12[3] * subpixx) >> 3 : 0xFF;
        }
        else if(nlastpixy && !nlastpixx) // last col
        {
            r = (r * (subpixy) + dst21[0] * (8 - subpixy)) >> 3;
            g = (g * (subpixy) + dst21[1] * (8 - subpixy)) >> 3;
            b = (b * (subpixy) + dst21[2] * (8 - subpixy)) >> 3;
            a = (b_images[i].bpp == 4) ? (a * (subpixy) + dst21[3] * (8 - subpixy)) >> 3 : 0xFF;
        }
        else if(nlastpixy && nlastpixx)
        {
            // T.T
            r = (((dst21[0] * (8 - subpixx) + dst22[0] * subpixx) >> 3) * (subpixy) + ((r * (8 - subpixx) + dst12[0] * subpixx) >> 3) * (8 - subpixy)) >> 3;
            g = (((dst21[1] * (8 - subpixx) + dst22[1] * subpixx) >> 3) * (subpixy) + ((g * (8 - subpixx) + dst12[1] * subpixx) >> 3) * (8 - subpixy)) >> 3;
            b = (((dst21[2] * (8 - subpixx) + dst22[2] * subpixx) >> 3) * (subpixy) + ((b * (8 - subpixx) + dst12[2] * subpixx) >> 3) * (8 - subpixy)) >> 3;
            a = (b_images[i].bpp == 4) ? (((dst21[3] * (8 - subpixx) + dst22[3] * subpixx) >> 3) * (subpixy) + ((a * (8 - subpixx) + dst12[3] * subpixx) >> 3) * (8 - subpixy)) >> 3 : 0xFF;
        }
        // alpha fix
        DWORD al = (a * b_images[i].alpha * (0xff - fadalpha));
        colorb = (al & 0xFF0000) << 8 | r << 16 | g << 8 | b;

        return colorb;
    }
    // usual color
//	if(mode[i]==0)
    return (colors[i] | alphas[i] << 24);
}
#endif

#ifdef _VSMOD // patch m008. distort
MOD_DISTORT::MOD_DISTORT()
{
    enabled = false;
    pointsx[0] = 1;
    pointsy[0] = 0;
    pointsx[1] = 1;
    pointsy[1] = 1;
    pointsx[2] = 0;
    pointsy[2] = 1;
}

bool MOD_DISTORT::operator == (MOD_DISTORT& md)
{
    return(enabled == md.enabled
           && pointsx[0] == md.pointsx[0]
           && pointsx[1] == md.pointsx[1]
           && pointsx[2] == md.pointsx[2]
           && pointsy[0] == md.pointsy[0]
           && pointsy[1] == md.pointsy[1]
           && pointsy[2] == md.pointsy[2]);
}
#endif

#ifdef _VSMOD // patch m011. jitter
MOD_JITTER::MOD_JITTER()
{
    seed = 0;
    offset = CRect(0, 0, 0, 0);
    period = 1;
    enabled = false;
}

bool MOD_JITTER::operator == (MOD_JITTER& mj)
{
    return(seed == mj.seed
           && offset == mj.offset
           && period == mj.period);
}

CPoint MOD_JITTER::getOffset(int64_t rt)
{
    if(!enabled) return CPoint(0, 0);
    if(period == 0) period = 1;
    int rseed = (seed + rt / period) * 100;

    srand(rseed);
    int xamp = (offset.left + offset.right);
    int xoffset = (xamp != 0) ? ((rand() % xamp) - offset.left) : 0;

    int yamp = (offset.bottom + offset.top);
    int yoffset = (yamp != 0) ? ((rand() % yamp) - offset.top) : 0;

    return CPoint((int)xoffset, (int)yoffset);
}
#endif