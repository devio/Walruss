# Walruss PC-Wahl 10 Attic

Collection of random stuff.

## Hold my beer

**holdmybeer.png** 

PC-Wahl vendor released a new version 170903 of his software. It only took a few hours to spot the differences and provide a new update-package that fits the newly introduced update mechanism. Their changelog was like "13.09.17: Changed to new update-system (digitally signed install packages)".  However, it is still broken, and after a few hours we took a screenshot of the hex-editor, to demonstrate that we were able to (manually) built a malicious update-package using a simple hex-editor.

This is the original tweet from September 14th, 2017 0:11 AM MEST:
<https://twitter.com/__ths__/status/908090844208648192>

and here we go:

```
$ sha1sum holdmybeer.png
b12a2d3d45d01cce9ee49a562a47d120bc168dcb *holdmybeer.png
```

### Technical details

The vendor realized that it is necessary to sign update packages. Instead of using signed MSI installers, which basically instruments the operating system to perform all the complicated crypto-verification, they build their own mechanism:

* They programmed a "Hello World" application, called `PC-Wahl-Paket1.ex_`. This is a valid PE executable. When executed, a dialog box is opened, telling the user, that "this program has to be digitally signed".
* `PC-Wahl-Paket1.ex_` is signed with a Comodo Code-Signing Certificate
* The well-known update-package format (.010 file) is used to archive and encrypt updates. This .010-file is embedded in `PC-Wahl-Paket1.ex_` at a known offset (0x49c00). The 32-bit integer at offset 0x49c00 is the lengths of the .010-file.
* The user is asked to manually verify, if `PC-Wahl-Paket1.ex_` is signed with a certificate by presenting a screenshot by the updater-program (programmaktualisierung10.exe).
* The updater asks the user, if the user saw a "Signature OK" text in the file properties. If so, the user is asked to click on "OK" in a dialog-box by programmaktualisierung10.exe.
* The updater (programmaktualisierung10.exe) opens the "Hello World" executable `PC-Wahl-Paket1.ex_` read-only, and extracts the .010 update archive file from offset 0x49c04.
* Afterwards, the .010 file is decrypted, extracted and installed.

There is a screencast video available, to demonstrate the exploitation of version 170913 update mechanism. In this video we used a certificate with "CN=regio iT gmbh", signed by a self-signed "C0MODO RSA Code Signing CA". However, the user probably does not care if it would have not been re-signed.

