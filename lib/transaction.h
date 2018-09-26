<?xml version="1.0" encoding='UTF-8'?>
<!DOCTYPE sect1 PUBLIC "-//OASIS//DTD DocBook V4.2//EN"
    "http://www.oasis-open.org/docbook/xml/4.2/docbookx.dtd">

<sect1 id="unicode" xreflabel="Using Unicode with MySQL++">
  <title>Using Unicode with MySQL++</title>

  <sect2 id="unicode-history">
    <title>A Short History of Unicode</title>
    <subtitle>...with a focus on relevance to MySQL++</subtitle>

    <para>In the old days, computer operating systems only dealt with
    8-bit character sets. That only allows for 256 possible characters,
    but the modern Western languages have more characters combined than
    that alone. Add in all the other languages of the world plus the
    various symbols people use in writing, and you have a real
    mess!</para>

    <para>Since no standards body held sway over things like
    international character encoding in the early days of computing,
    many different character sets were invented. These character sets
    weren&#x2019;t even standardized between operating systems, so heaven
    help you if you needed to move localized Greek text on a DOS box to
    a Russian Macintosh! The only way we got any international
    communication done at all was to build standards on top of the
    common 7-bit ASCII subset.  Either people used approximations like a
    plain &#x201C;c&#x201D; instead of the French &#x201C;&ccedil;&#x201D;,
    or they invented things like HTML entities
    (&#x201C;&amp;ccedil;&#x201D; in this case) to encode these additional
    characters using only 7-bit ASCII.</para>

    <para>Unicode solves this problem. It encodes every character used
    for writing in the world, using up to 4 bytes per character. The
    subset covering the most economically valuable cases takes two bytes
    per character, so most Unicode-aware programs deal in 2-byte
    characters, for efficiency.</para>

    <para>Unfortunately, Unicode was invented about two decades
    too late for Unix and C. Those decades of legacy created an
    immense inertia preventing a widespread move away from 8-bit
    characters. MySQL and C++ come out of these older traditions, and
    so they share the same practical limitations. MySQL++ currently
    doesn't have any code in it for Unicode conversions; it just
    passes data along unchanged from the underlying MySQL C API,
    so you still need to be aware of these underlying issues.</para>

    <para>During the development of the <ulink
    url="http://en.wikipedia.org/wiki/Plan_9_from_Bell_Labs">Plan
    9</ulink> operating system (a kind of successor to Unix) Ken
    Thompson <ulink
    url="http://www.cl.cam.ac.uk/~mgk25/ucs/utf-8-history.txt">invented</ulink>
    the <ulink url="http://en.wikipedia.org/wiki/UTF-8">UTF-8
    encoding</ulink>. UTF-8 is a superset of 7-bit ASCII and is
    compatible with C strings, since it doesn&#x2019;t use 0 bytes
    anywhere as multi-byte Unicode encodings do. As a result, many
    programs that deal in text will cope with UTF-8 data even though
    they have no explicit support for UTF-8. (Follow the last link above
    to see how the design of UTF-8 allows this.) Thus, when explicit
    support for Unicode was added in MySQL v4.1, they chose to make
    UTF-8 the native encoding, to preserve backward compatibility with
    programs that had no Unicode support.</para>
  </sect2>


  <sect2 id="unicode-unix">
    <title>Unicode on Unixy Systems</title>

    <para>Linux and Unix have system-wide UTF-8 support these days. If
    your operating system is of 2001 or newer vintage, it probably has
    such support.</para>

    <para>On such a system, the terminal I/O code understands UTF-8
    encoded data, so your program doesn&#x2019;t require any special code
    to correctly display a UTF-8 string. If you aren&#x2019;t sure
    whether your system supports UTF-8 natively, just run the
    <filename>simple1</filename> example: if the first item has two
    high-ASCII characters in place of the &#x201C;&uuml;&#x201D; in
    &#x201C;N&uuml;rnberger Brats&#x201D;, you know it&#x2019;s not
    handling UTF-8.</para>

    <para>If your Unix doesn&#x2019;t support UTF-8 natively, it likely
    doesn&#x2019;t support any form of Unicode at all, for the historical
    reasons I gave above. Therefore, you will have to convert the UTF-8
    data to the local 8-bit character set. The standard Unix function
    <function>iconv()</function> can help here. If your system
    doesn&#x2019;t have the <function>iconv()</function> facility, there
    is <ulink url="http://www.gnu.org/software/libiconv/">a free
    implementation</ulink> available from the GNU Project. Another
    library you might check out is IBM&#x2019;s <ulink
    url="http://icu.sourceforge.net/">ICU</ulink>. This is rather
    heavy-weight, so if you just need basic conversions,
    <function>iconv()</function> should suffice.</para>
  </sect2>


  <sect2 id="unicode-windows">
    <title>Unicode on Windows</title>

    <para>Each Windows API function that takes a string actually comes
    in two versions. One version supports only 1-byte &#x201C;ANSI&#x201D;
    characters (a superset of ASCII), so they end in 'A'. Windows also
    supports the 2-byte subset of Unicode called <ulink
    url="http://en.wikipedia.org/wiki/UCS-2">UCS-2</ulink>.  Some call
    these &#x201C;wide&#x201D; characters, so the other set of functions
    end in 'W'. The <function><ulink
    url="http://msdn.microsoft.com/library/en-us/winui/winui/windowsuserinterface/windowing/dialogboxes/dialogboxreference/dialogboxfunctions/messagebox.asp">MessageBox</ulink>()</function>
    API, for instance, is actually a macro, not a real function. If you
    define the <symbol>UNICODE</symbol> macro when building your
    program, the <function>MessageBox()</function> macro evaluates to
    <function>MessageBoxW()</function>; otherwise, to
    <function>MessageBoxA()</function>.</para>

    <para>Since MySQL uses the UTF-8 Unicode encoding and Windows uses
    UCS-2, you must convert data when passing text between MySQL++ and
    the Windows API. Since there&#x2019;s no point in trying for
    portability &mdash; no other OS I&#x2019;m aware of uses UCS-2
    &mdash; you might as well use platform-specific functions to do this
    translation. Since version 2.2.2, MySQL++ ships with two Visual C++
    specific examples showing how to do this in a GUI program.  (In
    earlier versions of MySQL++, we did Unicode conversion in the
    console mode programs, but this was unrealistic.)</para>

    <para>How you handle Unicode data depends on whether you&#x2019;re
    using the native Windows API, or the newer .NET API. First, the
    native case:</para>

    <programlisting>
// Convert a C string in UTF-8 format to UCS-2 format.
void ToUCS2(LPTSTR pcOut, int nOutLen, const char* kpcIn)
{
  MultiByteToWideChar(CP_UTF8, 0, kpcIn, -1, pcOut, nOutLen);
}

// Convert a UCS-2 string to C string in UTF-8 format.
void ToUTF8(char* pcOut, int nOutLen, LPCWSTR kpcIn)
{
  WideCharToMultiByte(CP_UTF8, 0, kpcIn, -1, pcOut, nOutLen, 0, 0);
}</programlisting>

    <para>These functions leave out some important error checking, so
    see <filename>examples/vstudio/mfc/mfc_dlg.cpp</filename> for the
    complete version.</para>

    <para>If you&#x2019;re building a .NET application (such as, perhaps,
    because you&#x2019;re using Windows Forms), it&#x2019;s better to use
    the .NET libraries for this:</para>

    <programlisting>
// Convert a C string in UTF-8 format to a .NET String in UCS-2 format.
String^ ToUCS2(const char* utf8)
{
  return gcnew String(utf8, 0, strlen(utf8), System::Text::Encoding::UTF8);
}

// Convert a .NET String in UCS-2 format to a C string in UTF-8 format.
System::Void ToUTF8(char* pcOut, int nOutLen, String^ sIn)
{
  array&lt;Byte&gt;^ bytes = System::Text::Encoding::UTF8->GetBytes(sIn);
  nOutLen = Math::Min(nOutLen - 1, bytes->Length);
  System::Runtime::InteropServices::Marshal::Copy(bytes, 0,
    IntPtr(pcOut), nOutLen);
  pcOut[nOutLen] = '\0';
}</programlisting>

    <para>Unlike the native API versions, these examples are complete,
    since the .NET platform handles a lot of things behind the scenes
    for us. We don&#x2019;t need any error-checking code for such simple
    routines.</para>

    <para>All of this assumes you&#x2019;re using Windows NT or one of
    its direct descendants: Windows 2000, Windows XP, Windows Vista, or
    any &#x201C;Server&#x201D; variant of Windows. Windows 95 and its
    descendants (98, ME, and CE) do not support UCS-2. They still have
    the 'W' APIs for compatibility, but they just smash the data down to
    8-bit and call the 'A' version for you.</para>
  </sect2>


  <sect2 id="unicode-refs">
    <title>For More Information</title>
      <para>The <ulink url="http://www.unicode.org/faq/">Unicode
      FAQs</ulink> page has copious information on this complex
      topic.</para>

      <para>When it comes to Unix and UTF-8 specific items, the <ulink
      url="http://www.cl.cam.ac.uk/~mgk25/unicode.html">UTF-8 and
      Unicode FAQ for Unix/Linux</ulink> is a quicker way to find basic
      information.</para>
    </sect2>
</sect1>
