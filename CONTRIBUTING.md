# Contributing to the MySQL++ Project

If you wish to make any changes to [the project’s files][home], here are
some rules and hints to keep in mind while you work.

[home]: https://tangentsoft.com/mysqlpp/


## <a id="fossil-anon" name="anon"></a> Fossil Anonymous Access

There are three ways to clone the repository anonymously using Fossil.
Each of these methods gets you a file called `mysqlpp.fossil` containing
the [abridged version history][avh] back to the project’s founding.

You only need to take one of these options, once per machine.
Thereafter, you will just be working with the cloned repository.

[avh]:  https://tangentsoft.com/mysqlpp/wiki?name=Abridged+Version+History


### One-Step Clone-and-Open <a id="one-step-open"></a>

The easiest way requires Fossil 2.14 or higher:

    $ fossil clone https://tangentsoft.com/mysqlpp
    $ cd mysqlpp

That gets you a clone of the `mysqlpp.fossil` repository plus a check-out
of the current trunk in a `mysqlpp/` directory alongside it. We recommend
that you do this in a directory like `~/src` so you don’t commingle
these files with other things in your current working directory.


### Open from URI <a id="open-uri"></a>

If you have Fossil 2.12 or 2.13, the next-easiest method is:

    $ mkdir -p ~/src/mysqlpp
    $ cd ~/src/mysqlpp
    $ fossil open https://tangentsoft.com/mysqlpp

This opens the repository referenced by that URI into the current
directory as `mysqlpp.fossil`, then opens that repo into that same
subdirectory.

You have to create the destination directory first with this method
because Fossil will refuse to spam a non-empty directory with the
check-out contents when opening the repo into a directory containing
other files unless you give it the `--force` flag.

Notice that the repo file ends up *inside* the check-out tree with this
method. This is because of a purposeful semantic difference in Fossil
between “open” and “clone.” It may seem strange to someone coming from
Git, but while we don’t want to get into the whys and wherefores here,
realize there is logic behind this choice.


### Separate Clone and Open <a id="sep-clone-open"></a>

The complicated method works with all versions of Fossil back to 2.1,
and it is the one we recommend to people who want to get involved with
the project, because it has [numerous advantages][cowf] over the easy
methods. We’ll explain those benefits in the context of the MySQL++
project later, but for now, the method is:

    $ mkdir -p ~/museum ~/src/mysqlpp/trunk
    $ fossil clone https://tangentsoft.com/mysqlpp ~/museum/mysqlpp.fossil
    $ cd ~/src/mysqlpp/trunk
    $ fossil open ~/museum/mysqlpp.fossil

[cowf]: https://fossil-scm.org/home/doc/trunk/www/ckout-workflows.md


## <a id="login"></a> Fossil Developer Access

If you have a developer account on the `tangentsoft.com/mysqlpp` Fossil
instance, just add your username to the URL like so:

    $ fossil clone https://USERNAME@tangentsoft.com/mysqlpp mysqlpp.fossil

If you’ve already cloned anonymously, simply tell Fossil about the new
sync URL instead:

    $ cd ~/src/mysqlpp/trunk
    $ fossil sync https://USERNAME@tangentsoft.com/mysqlpp

Either way, Fossil will ask you for the password for `USERNAME` on the
remote Fossil instance, and it will offer to remember it for you. If
you let it remember the password, operation from then on is scarcely
different from working with an anonymous clone, except that on checkin,
your changes will be sync’d back to the repository on tangentsoft.com if
you’re online at the time, and you’ll get credit under your developer
account name for the checkin.

If you’re working offline, Fossil will still do the checkin locally, and
it will sync up with the central repository after you get back online.
It is best to work on a branch when unable to use Fossil’s autosync
feature, as you are less likely to have a sync conflict when attempting
to send a new branch to the central server than in attempting to merge
your changes to the tip of trunk into the current upstream trunk, which
may well have changed since you went offline.

You can purposely work offline by disabling autosync mode:

    $ fossil set autosync 0

Until you re-enable it (`autosync 1`) Fossil will stop trying to sync
your local changes back to the central repo. In this mode, Fossil works
more like Git’s default mode, buying you many of the same problems that
go along with that working style. I recommend disabling autosync mode
only when you are truly going to be offline and don’t want Fossil
attempting to sync when you know it will fail.


## <a id="gda"></a> Getting Developer Access

We are pretty open about giving developer access to someone who’s
provided at least one good, substantial [patch](#patches) to the
software. If we’ve accepted one of your patches, just ask for a
developer account [on the forum][pfor].

[pfor]: https://tangentsoft.com/mysqlpp/forum


## <a id="tags" name="branches"></a> Working with Existing Tags and Branches

The directory structure shown in the [separate clone and
open](#sep-clone-open) sequence above is more complicated than strictly
necessary, but it has a number of nice properties.

First, it collects software projects under a common top-level
directory. I’ve used `~/src` for this example, but you are free to use any scheme
you like.

Second, the level underneath the project directory (`~/src/mysqlpp`) stores multiple separate
checkouts, one for each version the developer is actively working with at the moment,
so to add a few other checkouts, you could say:

    ~/museum/                  # Where one keeps fossils, right?
        mysqlpp.fossil
    ~/src/                     # Working tree for software projects
        mysqlpp/               # A directory for each project
            trunk/             # Primary working branch for MySQL++
            v2.3.2-modern/     # Checkout for another branch
            v3.2.3/            # Checkout for a tagged stable release

This gives you multiple independent checkouts, which allows you to
quickly switch between versions with “`cd`” commands. The alternative
(favored by Git and some other version control systems) is to use a
single working directory and switch among versions by updating that
single working directory in place. The problem is that this
invalidates all of the build artifacts tied to changed files, so you
have a longer rebuild time than simply switching among check-out
directories. Since disk space is cheap these days,
it’s better to have multiple working states and
just “`cd`” among them.

When you say `fossil update` in a check-out directory, you get the “tip”
state of that version’s branch. This means that if you created your
“`release`” check-out while version 3.2.4 was current and you say
“`fossil update`” today, you’ll get the release version 3.2.5 or later,
because both tags are on the `release` branch.  This shows an essential
difference between tags and branches in Fossil, which are at bottom
otherwise nearly identical: tags apply to a single commit only, while
branches are propagating tags, moving from one commit to the next until
explicitly canceled.

The project uses tags for [each released version][tags], and it
has [many working branches][brlist]. You can use any of those names in
“`fossil open`” and “`fossil update`” commands, and you can also use any
of [Fossil’s special check-in names][fscn].

[brlist]: https://tangentsoft.com/mysqlpp/brlist
[fscn]:   https://fossil-scm.org/home/doc/trunk/www/checkin_names.wiki
[fvg]:    https://fossil-scm.org/home/doc/trunk/www/fossil-v-git.wiki
[gitwt]:  https://git-scm.com/docs/git-worktree
[tags]:   https://tangentsoft.com/mysqlpp/taglist


## <a id="branching"></a> Creating Branches

Creating a branch in Fossil is scary-simple, to the point that those
coming from other version control systems may ask, “Is that really all
there is to it?” Yes, really, this is it:

    $ fossil ci --branch new-branch-name

That is to say, you make your changes as you normally would; then when
you go to check them in, you give the `--branch` option to the
`ci/checkin` command to put the changes on a new branch, rather than add
them to the same branch the changes were made against.

While developers with login rights to the Fossil instance are
allowed to check in on the trunk at any time, we recommend using
branches whenever you’re working on something experimental, or where you
can’t make the necessary changes in a single coherent checkin. A good
example is an API or ABI breakage: those should go on a branch rather
than on trunk, since they require discussion before we merge them down
to trunk.

One of this project’s core principles is that `trunk` should always
build without error, and it should always function correctly. That’s an
ideal we have not always achieved, but we do always *try* to achieve it.

Contrast branches, which project developers may use to isolate work
until it is ready to merge into the trunk. It is okay to check work in
on a branch that doesn’t work, or doesn’t even *build*, so long as the
goal is to get it to a point that it does build and work properly before
merging it into trunk.

Here again we have a difference with Git: because Fossil normally syncs
your work back to the central repository, this means we get to see the
branches you are still working on. This is a *good thing*. Do not fear
committing broken or otherwise bad code to a branch. [You are not your
code.][daff] We are software developers, too: we understand that
software development is an iterative process, that not all ideas
spring forth perfect and production-ready from the fingers of its
developers. These public branches let your collaborators see what
you’re up to; they may be able to lend advice, to help with the work, or
to at least be unsurprised when your change finally lands in trunk.

Fossil fosters close cooperation, whereas Git fosters wild tangents that
never come back home.

Jim McCarthy (author of [Dynamics of Software Development][dosd]) has a
presentation on YouTube that touches on this topic at a couple of
points:

*   [Don’t go dark](https://www.youtube.com/watch?v=9OJ9hplU8XA)
*   [Beware of a guy in a room](https://www.youtube.com/watch?v=oY6BCHqEbyc)

Fossil’s sync-by-default behavior fights these negative tendencies.

Project developers are welcome to create branches at will. The
main rule is to follow the branch naming scheme: all lowercase with
hyphens separating words. See the [available branch list][brlist] for
examples to emulate.

If you have checkin rights on the repository, it is generally fine to
check things in on someone else’s feature branch, as long as you do so
in a way that cooperates with the purpose of that branch. The same is
true of `trunk`: you should not check something in directly on the trunk
that changes the nature of the software in a major way without
discussing the idea first. This is yet another use for branches: to
make a possibly-controversial change so that it can be discussed before
being merged into the trunk.

[daff]: http://www.hanselman.com/blog/YouAreNotYourCode.aspx
[dosd]: http://amzn.to/2iEVoBL


## <a id="special"></a> Special Branches

Most of the branches in the project are feature branches of the
sort described in the previous section: an isolated line of development
by one or more of the project’s developers to work towards some new
feature, with the goal of merging that feature into the `trunk` branch.

There are a few branches in the project that are special, which are
subject to different rules than other branches:

*   **<code>release</code>** — One of the steps in the
    [release process][relpr] is to merge the stabilized `trunk` into the
    `release` branch, from which the release tarballs and binary OS
    images are created. Only the project’s release manager — currently
    Warren Young — should make changes to this branch.

*   **<code>bogus</code>** or **<code>BOGUS</code>** — Because a branch
    is basically just a label for a specific checkin, Fossil allows the tip
    of one branch to be “moved” to another branch by applying a branch
    label to that checkin. We use this label when someone makes a
    checkin on the tip of a branch that should be “forgotten.” Fossil
    makes destroying project history very difficult, on purpose, so
    things moved to the “bogus” branch are not actually destroyed;
    instead, they are merely moved out of the way so that they do not
    interfere with that branch’s normal purpose.

    If you find yourself needing to prune the tip of a branch this way,
    the simplest way is to do it via the web UI, using the checkin
    description page’s “edit” link. You can instead do it from the
    command line with the `fossil amend` command.

[relpr]: https://tangentsoft.com/mysqlpp/doc/trunk/RELEASE-CHECKLIST.txt


## <a id="forum"></a> Discussion Forum

The “[Forum][pfor]” link at the top of the Fossil web interface is for
both end-user discussions and internal MySQL++ project developer
discussions. If you come across the old mailing list, we don’t use that
any more. We also prefer Fossil forum and ticket postings to GitHub
issues and such.

You can sign up for the forum without having a developer login, and you
can even post anonymously. If you have a login, you can [sign up for
email alerts][alert] if you like.

Keep in mind that posts to the Fossil forum are treated much the same
way as ticket submissions and wiki articles. They are permanently
archived with the project. The “edit” feature of Fossil forums just
creates a replacement record for a post, but the old post is still
available in the repository. Don’t post anything you wouldn’t want made
part of the permanent record of the project!

[alert]: https://tangentsoft.com/mysqlpp/alerts



## <a id="bootstrap"></a>Bootstrapping the Library

When you check out MySQL++ from Fossil, there are a lot of things
"missing" as compared to a distributed tarball, because the Fossil
repository contains only source files, no generated files.  The
process that turns a fresh MySQL++ repository checkout into
something you can build and hack on is called bootstrapping.

Boostrapping is best done on a modern Unix type platform: Linux, OS X,
BSD, Solaris...any version released since 2005 or so.

It's possible to [bootstrap MySQL++ on Windows](#winbs), but it’s much
harder.

Two of the tools you need to do this are commonly available on Unixy
systems, at least as an option: Perl 5, and GNU Autoconf 1.59 or higher.
If they're not installed, you can probably run your system's package
manager to install suitable versions.

There's a third tool you'll need to bootstrap MySQL++ called
[Bakefile][bf]. The syntax used in `mysql++.bkl` requires at least
Bakefile 0.2.5 or higher, which in turn requires Python 2.3 or higher to
run. You may require a newer version of Bakefile to support newer OSes
and Python versions; we've tested with versions up to 0.2.11
successfully.

Do not use any of the Bakefile 1.x versions: it’s an incompatible
change, and we currently have no intention to switch from Bakefile 0.x.

Once you have all the tools in place, you can bootstrap MySQL++ with a
Bourne shell script called `bootstrap`, which you get as part of the
Fossil checkout. It's fairly powerful, with many options.  For most
cases, it suffices to just run it without any arguments:

    $ ./bootstrap

For more unusual situations, here's the complete usage:

    $ ./bootstrap [no{doc,ex,lib,opt}] [pedantic] [bat] [configure flags]

Arguments:

*   `nodoc`

    The documentation won't be considered a prerequisite for building
    the distribution tarball. This is useful on systems where the
    documentation doesn't build correctly, and you only need to make a
    binary RPM. That process requires a tarball, but doesn't need the
    documentation. Don't distribute the tarball or SRPM that results, as
    they are no good for any other purpose.

*   `noex`

    The generated `Makefiles` and project files won't try to build any of
    the examples.

*   `nolib`

    The generated `Makefiles` and project files won't try to build the
    MySQL++ library.

*   `nomaint`

    Turn off "maintainer mode" stuff in the build. These are features
    used only by those building MySQL++ from Fossil. The `dist` build
    target uses this when creating the tarball, because it reduces the
    build time somewhat.

*   `noopt`

    Compiler optimization will be turned off. (This currently has no
    effect on the generated MinGW Makefile or the Visual C++ project
    files.)

*   `pedantic`

    Turns on all of GCC's warnings and portability checks.  We use this
    as part of our [release process](./RELEASE-CHECKLIST.txt).

*   `bat`

    Runs `bootstrap.bat` via `cmd.exe` for you, passing along equivalent
    options to any of the "*no*" options you give before it.

    Only the "*no*" options above have an effect on the generated build
    system files when you give "`bat`".  In particular, the files
    generated by `bootstrap.bat` make no distinction between "pedantic"
    and normal builds.

    Passing `bat` stops all command line processing in the `bootstrap`
    script, so if you also pass some of the other options, "`bat`" must
    be last.
    
    The `bootstrap.bat` script is useful only when you intend to build
    MySQL++ with MinGW or Visual C++, and you are using Cygwin only as a
    command line environment.  If you intend to build MySQL++ with
    Cygwin's GCC toolchain, you must not give this option, else you will
    not end up with the necessary build system files.

    One advantage of this feature is that the commands necessary to
    achieve a given effect with `bootstrap.bat` when run via `bootstrap`
    are shorter than when you run the batch file directly.

    Another advantage is that this low-strength version of the bootstrap
    script runs faster than the full-strength form, because it produces
    fewer files.

    Finally, running `bootstrap.bat` indirectly like this lets you avoid
    using `cmd.exe`, a command shell greatly inferior to any of those
    available for Cygwin.

*   `configure` script options

    As soon as the bootstrap script sees an option that it doesn't
    understand, it stops processing the command line.  Any subsequent
    options are passed to the `configure` script. See
    [README-Unix.txt][rmu] for more on `configure` script options.

[bf]:  http://bakefile.org/
[rmu]: https://tangentsoft.com/mysqlpp/file/README-Unix.txt


## <a id="winbs"></a>Bootstrapping the Library Using Only Windows

The thing that makes bootstrapping on Windows difficult is that one of
the required steps uses a Unix-centric tool, Autoconf.  This section
gives alternatives for either getting Autoconf working on Windows or
avoiding the need for it.

The thing Autoconf does that's relevant to Windows builds of MySQL++
is that it substitutes the current MySQL++ version number into several
source files. This allows us to change the version number in just one
place — `configure.ac` — and have it applied to all these other places.
Until you do this step, a Fossil checkout of MySQL++ won't build,
because these files with the version numbers in them won't be generated.


### Option 1: Copy the generated files over from a released version

Only one of these generated files is absolutely critical to allowing
MySQL++ to build: `lib/mysql++.h`. So, the simplest option you have to
bootstrap MySQL++ entirely on Windows is to copy `lib/mysql++.h` over
from a released version of MySQL++. While you're doing that, you might
copy over the other such generated files:

    install.hta
    mysql++.spec
    doc/userman/userman.dbx
    lib/Doxyfile

Having done that, you can complete the bootstrapping process by running
`bootstrap.bat`. It has the same purpose as the Bourne shell script
described [above](#bootstrap), but with a different and simpler usage:

    C:\> bootstrap.bat [bakefile-options]

Any options passed are passed as-is to Bakefile. This is normally used
to pass `-D` options to affect the generated build system output files.


### Option 2: Cygwin

If you'd like to hack on MySQL++ entirely on Windows and have all the
build freedoms enjoyed by those working on Unixy platforms, the simplest
solution is probably to [install Cygwin][cyg]. It doesn’t matter whether
you use the 32-bit or 64-bit version, for our purposes here.

While in the Cygwin setup program, you will have to add the Autoconf and
Perl 5 packages, which aren't installed in Cygwin by default.  Autoconf
is in the Devel category, and Perl 5 in the Interpreters category.

You will also need to install the native Windows binary version of
[Bakefile][bf].  Don't get the source version and try
to build Bakefile under Cygwin; it won't work. The Windows binary
version of Bakefile includes an embedded version of Python, so you won't
need to install Cygwin's Python.

Having done all this, you can follow the Unix bootstrapping
instructions in the previous section.

[cyg]: http://cygwin.com/


### Option 3: Windows Subsystem for Linux (WSL)

If you’re on Windows 10, you have the option of [installing WSL][WSL], a
lightweight Linux kernel and user environment that runs atop Windows.
This is fundamentally different technology than Cygwin, but the
user-level effect of it is the same as far as MySQL++’s build system
goes.

Assuming you use the default Ubuntu enviroment atop WSL, the [standard
bootstrapping process](#bootstrap) applies, after you install the needed
tools:

    $ sudo apt install build-essential perl libmysqlclient-dev

You will also need to install the legacy [Bakefile 0.2.x][bf] version.
If there is a `bakefile` package available at all, it is likely for the
incompatible 1.x series.

[WSL]: https://docs.microsoft.com/en-us/windows/wsl/install-win10


### Option 4: ["Here's a nickel, kid, get yourself a better computer."][dc]

Finally, you might have access to a Unixy system, or the ability to set
one up. You don't even need a separate physical computer, now that
virtual machine techology is free.

Given such a machine, you'd do the Fossil checkout of MySQL++ on that
machine, then bootstrap it there using the instructions in the previous
section, and copy the generated files back to the Windows box.

[dc]: http://tomayko.com/writings/that-dilbert-cartoon


## <a id="build-system"></a> Manipulating the Build System Source Files

One of the things the bootstrapping system described [above](#bootstrap)
does is produces various types of project and make files from a small
number of source files. This system lets us support many platforms
without having to maintain separate build system files for each
platform.

[Bakefile][bf] produces most of these project and make
files from a single source file called [`mysql++.bkl`][bkl].

Except for small local changes, it's best to change `mysql++.bkl` and
"re-bake" the project and make files rather than change those files
directly. You can do this with the bootstrap scripts covered above. On
Windows, if all you've changed is `mysql++.bkl`, you can use
`rebake.bat` instead, which doesn't try to do as much as
`bootstrap.bat`.

Bakefile produces finished project files for Visual C++ and Xcode and
finished `Makefiles` for MinGW. It also produces `Makefile.in`, which is
input to GNU Autoconf along with `configure.ac` and `config/*`. You may
need to change these latter files in addition to or instead of
`mysql++.bkl` to get the effect you want.  Running bootstrap
incorporates changes to all of these files in the GNU Autoconf output.

While Bakefile's documentation isn't as comprehensive as it
ought to be, you can at least count on it to list all of the
available features. So, if you can't see a way to make Bakefile
do something, it's likely it just can't do it. Bakefile is a
high-level abstraction of build systems in general, so it'll never
support all the particulars of every odd build system out there.

[bkl]: https://tangentsoft.com/mysqlpp/file/mysql%2B%2B.bkl



## <a id="patches"></a> Submitting Patches

If you do not have a developer login on the project repository, you can
still send changes to the project.

The simplest way is to say this after developing your change against
trunk:

    $ fossil diff > my-changes.patch

Then paste that into a [forum post][pfor] using a [fenced code
block][fcb]. We will also accept trivial patches not needing discussion
as text or attachments on [a Fossil ticket][tkt].

If you're making a patch against a MySQL++ distribution tarball, you can
generate a patch this way:

    $ diff -ruN mysql++-olddir mysql++-newdir > mychange.patch

The `diff` command is part of every Unix and Linux system, and should be
installed by default. If you're on a Windows machine, GNU diff is part
of [Cygwin](http://cygwin.com/) and [WSL]. Fossil is also available for
all of these systems. There are no excuses for not being able to make
unified diffs. :)

[fcb]: https://www.markdownguide.org/extended-syntax#fenced-code-blocks
[tkt]: https://tangentsoft.com/mysqlpp/tktnew


#### Bundles Instead of Patches

If your change is more than a small patch, `fossil diff` might not
incorporate all of the changes you have made. The old unified `diff`
format can’t encode branch names, file renamings, file deletions, tags,
checkin comments, and other Fossil-specific information. For such
changes, it is better to send a [Fossil bundle][fb]:

    $ fossil set autosync 0                # disable autosync
    $ fossil checkin --branch my-changes
      ...followed by more checkins on that branch...
    $ fossil bundle export --branch my-changes my-changes.bundle

After that first `fossil checkin --branch ...` command, any subsequent
`fossil ci` commands will check your changes in on that branch without
needing a `--branch` option until you explicitly switch that checkout
directory to some other branch. This lets you build up a larger change
on a private branch until you’re ready to submit the whole thing as a
bundle.

Because you are working on a branch on your private copy of the
project’s Fossil repository, you are free to make as many checkins as
you like on the new branch before giving the `bundle export` command.

Once you are done with the bundle, upload it somewhere public and point
to it from a forum post or ticket.

[fb]: https://fossil-scm.org/home/help?cmd=bundle


#### Contribution Licensing

Because MySQL++ uses the “[viral]” LGPL license, you do not have to
declare a release of copyright or explicit license: a public diff
against our source base is inherently made available under the same
license.

[viral]: https://en.wikipedia.org/wiki/Viral_license


#### <a id="ghm"></a> Can I Use GitHub Instead?

Although MySQL++ does have a [GitHub mirror][ghm], it
is intended as a read-only mirror for those heavily tied into Git-based
tooling. You’re welcome to send us a PR anyway, but realize that what’s
going to happen on the back end is that we’ll generate a patch, apply it
to the Fossil repo by hand, test it, and then commit it to the
repository under one of our existing Fossil developer accounts. Only
then do we update the mirror so that the change appears on GitHub; thus,
you don’t get GitHub credit for the PR. You avoid these problems by
simply asking for a developer account on the Fossil repo, so you can
commit there instead.

This is not simply because setting up bidirectional mirroring is
difficult, it is actually [impossible to achieve 100% fidelity][ghlim]
due to limitations of Git and/or GitHub. If you want a faithful clone of
the project repo, or if you wish to contribute to the project’s
development with full credit for your contributions, it’s best done via
Fossil, not via GitHub.

[ghlim]: https://fossil-scm.org/home/doc/trunk/www/mirrorlimitations.md
[ghm]:   https://github.com/tangentsoft/mysqlpp/


## <a id="style"></a> The MySQL++ Code Style

Every code base should have a common code style. Love it or
hate it, here are MySQL++'s current code style rules:


### Source Code

File types: `ac`, `cpp`, `h`, `in`, `m4`, `pl`

-   Tabs for indents, size 4

-   Unix line endings. Any decent programmer's editor can cope with
    this, even on Windows.

-   C/C++ rules:

    -   Base whitespace style is AT&Tish: K&R/Stroustrup, plus a little
        local spice. If you have the indent(1) program, the command is:

            indent -kr -nce -cli4 -ss -di1 -psl -ts4 FILES...

        That is, don't cuddle else, indent case statement labels, space
        before semicolon with empty loop body, no extra space between a
        variable type and name, return value of function on separate
        line from rest of definition.

    -   Class names are in `CamelCase`, uppercased first letter

    -   Method names are in `all_lower_case_with_underscores()`;
        ditto most other global symbols.

    -   Macro names are in `ALL_UPPERCASE_WITH_UNDERSCORES`

    -   Doxygen comment for all public declarations, unless there is a
        very good reason to keep the thing undocumented.

-   Perl and shell script rules are more or less the same as for C/C++,
    to the extent this makes sense.


### XML/HTML Dialects

File types: `bkl`, `dbx`, `hta`

-   Spaces for indents, size 2. Shallow indents due to the high level of
    nesting occurring in such files, and spaces because they're not as
    annoying at shallow indent levels in editors that don't treat space
    indents like tabs.

-   Unix line endings. Again, these are intended to be viewed in a
    programmer's text editor, which should work with Unix line endings
    no matter the platform.


### Plain Text Files

File types: `txt`

-   Spaces for indents, size 4. Spaces because such files are often
    viewed in Notepad and similarly crippled text editors which use a
    default indent level of 8.

-   DOS line endings, again for the Notepad reason. And on modern Unixy
    platforms, the tools cope with DOS line endings reasonably well.
    Better than the converse, anyway.


When in doubt, mimic what you see in the current code. When still in
doubt, ask on [the forum][pfor].


## <a id="testing"></a> Testing Your Proposed Change

MySQL++ includes a self-test mechanism called `dtest`. It's a Bourne
shell script, run much like `exrun`:

    $ ./dtest [-s server_addr] [-u user] [-p password]

This automatically runs most of the examples, captures the outputs to a
file, and then compares that to a known-good run's outputs, stored in
`bmark.txt`. So, before you submit a patch, run `dtest` to see if
anything has changed. If something has and you can't account for it, it
represents a problem that you'll have to fix before submitting the
patch. If it gives an expected change, remove `bmark.txt`, re-run
`dtest`, and include the `bmark.txt` diffs in your patch. This
communicates to us the fact that you know there are differences and want
the patch evaluated anyway. Otherwise, we are likely to view the change
in the program outputs as a bug or regression.

`dtest` also runs all of the unit tests in `test/*`. The purpose of
`test/*` is different from that of `examples/*`:

-   `test/*` are unit tests: each tests only one MySQL++ class,
    independent of everything else. Because DB access requires several
    MySQL++ classes to cooperate, a unit test never accesses a database;
    hence, no unit test needs DB connection parameters.  We will never
    get 100% code coverage from `test/*` alone.

-   `examples/*` can be thought of as integration tests: they test many
    pieces of MySQL++ working together, accessing a real database
    server. In addition to ensuring that all the pieces work together
    and give consistent results from platform to platform and run to
    run, it also fills in gaps in the code coverage where no suitable
    `test/*` module could be created.

-   `test/*` programs always run silently on success, writing output
    only to indicate test failures. This is because they're usually only
    run via `dtest`.

-   `examples/*` are always "noisy," regardless of whether they succeed
    or fail, because they're also run interactively by people learning
    to use MySQL++.

Patches should include tests if they introduce new functionality or fix
a bug that the existing test coverage failed to catch.  If the test is
noisy, needs DB access, or tests multiple parts of the library at once,
it goes in `examples/*`. If your change affects only one class in
MySQL++ and testing it can be done without instantiating other MySQL++
classes — other than by composition, of course — it should go in
`test/*`.

In general, prefer modifying an existing `examples/*` or `test/*`
program.  Add a new one only if you're introducing brand new
functionality or when a given feature currently has no test at all.

Beware that the primary role of the examples is to illustrate points in the
user manual. If an existing example does something similar to what a
proper test would need to do and the test doesn't change the nature of
the example, don't worry about changing the example code. If your test
would change the nature of the example, you either need to do the test
another way or also submit a change to `doc/userman/*.dbx` that
incorporates the difference.


## Adding Support for a Different Compiler

As described above, MySQL++ uses the Bakefile system for creating
project files and makefiles. This allows a single change to propagate
across all build systems we support. In the past, we used more ad-hoc
systems, and we'd frequently forget to update the build system for a
particular platform, so that at any given time, at least one target was
likely to be broken. We will therefore resist any change that requires
that we go back to distributing manually-maintained build system files
not generated from a single source.

If MySQL++ doesn't currently ship with project files or makefiles tuned
for your compiler of choice, you need to work through the Bakefile
mechanism to add support. We're not willing to do ad-hoc platform
support any more, so please don't ask if you can send us project files
instead; we don't want them.

If you want to port MySQL++ to another platform, we need to be confident
that the entire library works on your platform before we'll accept
patches. In the past, we've had broken ports that were either missing
important library features, or which crashed when built in certain ways.
Few people will knowingly use a crippled version of MySQL++, since there
are usually acceptable alternatives.  Therefore, such ports become
maintenance baggage with little compensating value.


## <a name="private"></a> Maintaining a Private Repository

Although Fossil tries to sync changes back to the [MySQL++ Fossil
repository][home] by default, it is possible to maintain a private
repository that simply pulls changes in occasionally.

The first step is to turn off the auto-sync feature:

     $ fossil set autosync off

...or set it so it only pulls from the central repo without trying to push
local changes, making it complain that you don’t have commit capability:

     $ fossil set autosync pullonly

Then I recommend that you make any local changes on a branch:

    ...hack, hack, hack...
    $ fossil ci --branch my-local-branch

After you give the `--branch` option on a checkin, Fossil automatically
switches your local checkout to that branch, so that all further
checkins can be made without the `--branch` option.  To get back to the
trunk, you'd say `fossil up trunk`, but under this workflow, the need
for that will be rare.

When something happens on the official trunk on `tangentsoft.com` that
you want pulled into your private repository, say:

    $ fossil pull
    $ fossil merge trunk

The first command pulls all remote changes into your local clone, but
since those changes don't affect your private branch, you won't see any
immediate change. The second attempts to merge the trunk branch's
changes since the last branch or merge point into your private branch.

Whether the merge is successful or not, Fossil does not immediately
modify your clone, only the working checkout directory. You must then
say `fossil ci` once you're happy with the merge. Until then, all the
usual Fossil commands like `fossil diff` and `fossil status` will help
you come to that decision.

If you ever decide to contribute your private branch to the MySQL++
project, there are a couple of easy ways to achieve that. Ask about it
on [the forum][pfor] if you find yourself in this situation.

If you want to ensure that your private branch *never* syncs with the
public project, even if you later get a developer account on it, you can
go a step further and use Fossil’s [private branch feature][pbr].

[home]: https://tangentsoft.com/mysqlpp/
[pbr]:  https://fossil-scm.org/home/doc/trunk/www/private.wiki
