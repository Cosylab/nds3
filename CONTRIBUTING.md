# Contribute to NDS
Thank you for your interest in contributing to NDS!

It is possible to contribute in multiple ways. You can for example file a bug
report, improve the documentation or improve the implementation. We are very
interested to hear from you if you considered to or considered not to use NDS
for your driver development.

If you have an idea for improvement of NDS we welcome you to open an issue in
[the issue tracker](https://github.com/Cosylab/nds3/issues). Opening an issue
before starting any development will increase the success rate of that
contribution to eventually get merged. When the overall design of the change
has been decided fork the repository and implement the changes in your fork.
Finally when you want feedback on the code, open a pull request.

## General guidelines
- Prefix your PR subject with "[WIP]" if it isn't ready to be merged.
- Rebase/Squash your commits to reasonably sized commits.
   * BUT: Don't rebase your branch until code reviewing is done. This makes it
     easier to follow the discussions in the PR.
   * Finally, every commit should be self-contained and should build.
- Write descriptive commit messages.
   * First line should be in imperative form, starting with a capital letter
     and not end with punctuation. Same rules as for a header. It should
     complete the sentance "If applied, this commit will ..."
   * Second line should always be empty.
   * The rest of the message should explain the introduced changes in more
     detail.
- Follow the same coding style as the rest of the code.
- We currently target C++11, so please avoid GNU/MSVC specifics or features
  from later C++ standards.
