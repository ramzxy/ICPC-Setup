# NWERC desktop practice on Proxmox

Use a full Linux VM as the team's practice workstation. Windows and Linux
computers can both access its desktop through the Proxmox console; neither
client needs a local Linux installation. Keep DOMjudge and its judgehost separate
from this contestant VM.

This is a provisioning guide, checked against the public documentation on
2026-11-10. The user's first Proxmox trials exposed a truncated uploaded image;
desktop boot from the complete image still needs verification.

## What to create

Create an empty VM named `nwerc-practice-base`, with no installation ISO and no
automatically created OS disk. Add an EFI disk, then import the official image
as the OS disk as described below.

| Proxmox setting | Suggested value |
| --- | --- |
| Guest | Linux, x86-64 |
| Machine / firmware | Q35 / OVMF (UEFI) |
| EFI disk | EFI type `4m`; Pre-Enroll Keys disabled (Secure Boot off) |
| CPU | 1 socket, 4 cores, type `host` |
| Memory | 8192 MiB, ballooning disabled |
| Disk controller | VirtIO SCSI Single |
| OS disk | Import the whole decompressed ICPC `.img`, attach as SCSI0 |
| Storage | Snapshot-capable storage; budget at least 64 GiB per workstation, plus download/extraction space |
| Display | SPICE (QXL); browser noVNC is fine for initial setup |
| Network | VirtIO on a private LAN/VLAN with DHCP |

CPU, RAM and storage are comfortable practice allocations, not announced contest
hardware specifications. Leave host resources available for IDEs and the other
VMs/services. CPU `host` is suitable for a VM staying on this node; use a common
supported CPU model instead if migration between different CPUs is required.
No GPU passthrough, nested virtualization or TPM is needed for this workflow.

The image needs substantial temporary space for both the compressed download
and extracted raw disk. Check the extracted image size before importing. Keep
enough additional VM storage for snapshots and practice files; thin provisioning
does not eliminate the need for free physical space.

See the [official Proxmox VM guide](https://github.com/proxmox/pve-docs/blob/master/qm.adoc)
and [command reference](https://github.com/proxmox/pve-docs/blob/master/generated/qm.1-synopsis.adoc)
for firmware, storage, import and console settings.

## Import the official ICPC image

The download contains a complete, preconfigured bootable OS disk image, including
the desktop and contest tools. It is not a conventional OS installer. The ICPC
instructions describe running the OS directly from the imaged USB drive; the
[World Finals instructions](https://image.icpc.global/wf2026/ImageBuildInstructions.html)
also describe copying the whole disk onto an internal drive with Clonezilla.
For Proxmox, import the image as the VM's boot disk.

NWERC uses a modified World Finals image. Start with the
[official ICPC download and installation page](https://image.icpc.global/icpc2026/ImageBuildInstructions.html),
which currently links `icpc2026-2026-05-29_image-amd64.img.gz` (about 7.9 GB).
Download that image and its published SHA256 file into a staging directory with
enough free space. Transfer them to the Proxmox node if downloaded elsewhere.
Proxmox also accepts `.img` files through its **ISO Images** upload dialog, so
SCP is optional. Extract the `.gz` first for that route. This uploads the file;
it does not turn the OS disk image into an installer. Import it as a VM disk
after uploading. The commands below provide the CLI route; current Proxmox
versions also offer guest disk-image uploads under **Import** content.
See the [Proxmox upload implementation](https://github.com/proxmox/pve-manager/blob/master/www/manager6/window/UploadToStorage.js).

In the staging directory on the Proxmox node, verify the download against the
[published checksum](https://image.icpc.global/icpc2026/SHA256SUMS.amd64.txt):

```sh
sha256sum -c SHA256SUMS.amd64.txt
```

Proceed only if the selected image reports `OK`. For the image above the
published SHA256 is
`06a0f4d6ec3c0a0b67b1ec49a0aa41225962fef88ff7bc7c4de10006015fefd7`.

Decompress a copy, then import it into the empty VM. Replace `VMID`, `STORAGE`
and `/path/` with the chosen VM ID, VM-disk storage name and staging directory:

```sh
gzip -dk icpc2026-2026-05-29_image-amd64.img.gz
stat -c '%s' icpc2026-2026-05-29_image-amd64.img
sha256sum icpc2026-2026-05-29_image-amd64.img
```

For this exact release, the complete raw image must be **29,884,416,000 bytes**.
Its uncompressed SHA256 is
`e091ae32875073b7594e562d4f33b3888e61ad9df2b01b363a610bc48f51da26`.
This raw checksum was calculated locally from a complete decompression of the
organizer-checksummed `.gz`, with gzip's integrity check passing. It is not a
separately published organizer checksum. If extracting on Windows and uploading
the `.img`, check both its size and SHA256 again on the Proxmox node.

Only import after the size and checksum match:

```sh
qm disk import VMID /path/icpc2026-2026-05-29_image-amd64.img STORAGE
```

No intermediate qcow2 conversion is required. `qm importdisk` is an alias for
`qm disk import`. In the VM's **Hardware** tab, attach the resulting **Unused
Disk** as SCSI0. Use the imported volume actually shown by Proxmox rather than
guessing its name. Under **Options > Boot Order**, enable SCSI0 and put it first.
To give the imported disk the intended 64 GiB capacity, with the VM stopped:

```sh
qm disk resize VMID scsi0 64G
```

Then start the VM and open its console. This increases virtual capacity; growing
the guest filesystem is a separate operation.

### What went wrong in the initial trial

The earlier uploaded `.img` was only **27,263,500,288 bytes**, missing
**2,620,915,712 bytes (about 2.44 GiB)** from the complete source. Its root
filesystem extends to disk byte 29,883,367,424. Expanding the truncated disk
adds empty space and cannot restore the missing file contents. Reusing that
incomplete `.img` for a fresh VM repeats the problem.

The observed boot failures included GRUB disk-boundary/kernel errors, root
filesystem corruption and a crashing LightDM greeter with missing GSettings
schema data. These are consistent with missing filesystem data, though the
precise extraction/upload step that truncated the earlier copy is not known.
Use a complete, verified source for a fresh import before judging whether the
image's kernel or graphics stack is compatible with the VM. A filesystem repair
can fix metadata while leaving missing file contents unrecovered.

If the firmware reports a Secure Boot or bad-shim-signature error, restart with
the console open and press Esc at the firmware screen. Open **Device Manager >
Secure Boot Configuration**, uncheck **Attempt Secure Boot**, save with F10 and
reset. Keep OVMF/UEFI selected. This is useful if the EFI disk was originally
created with Pre-Enroll Keys enabled.

The generic ICPC instructions document physical USB booting, not Proxmox.
The imported disk already contains the OS; no separate OS installation step is
expected. Confirm that the desktop boots and a file survives a full shutdown
and restart before freezing the practice baseline. Inspect the disk layout
before resizing: increasing virtual disk capacity alone does not grow guest
partitions or filesystems.

## Match the NWERC workstation

The [NWERC 2026 system specification](https://2026.nwerc.eu/system) takes precedence
over generic World Finals defaults:

| Component | NWERC target |
| --- | --- |
| OS / desktop session | Ubuntu 24.04.3 LTS / Xfce 4.18 |
| C++ | GCC 13.3.0, GNU C++20 |
| Python | PyPy 7.3.15 implementing Python 3.9.18 |
| Main IDE choices | VS Code 1.93.1, PyCharm Community 2024.2.3, CLion 2024.2.2 |
| Terminal editors | Nano 7.2, Vim 9.1 |

Check what actually boots before installing replacements. The ICPC
[overview](https://image.icpc.global/icpc2026/environment.html) and
[package manifest](https://image.icpc.global/icpc2026/filesystem.manifest.amd64.txt)
differ: the overview mentions GNOME/GCC 13.2, while the manifest includes Xfce
4.18 and GCC 13.3. Select the Xfce session at login if already installed. Install
missing components only after recording the existing versions, then recheck.
Use the published IDE versions and extensions for the editor you intend to use;
avoid personal settings sync and automatic IDE/extension upgrades in the frozen
practice baseline.

Run these inside a terminal in the guest:

```sh
cat /etc/os-release
printf '%s\n' "$XDG_CURRENT_DESKTOP"
xfce4-session --version
g++ -dumpfullversion
pypy3 --version
pypy3 -c 'import platform, sys; assert platform.python_implementation() == "PyPy"; assert sys.version_info[:3] == (3, 9, 18); assert sys.pypy_version_info[:3] == (7, 3, 15)'
type -a python3 pypy3
```

Also verify the chosen editor and interpreter selection inside the IDE. Python
submissions must be tested with `pypy3`; `python3` may be Ubuntu's newer CPython.
When typing the printed `rp` function for this VM, use `pypy3` in place of
`python3`. Do not replace Ubuntu's system Python. The notebook still needs a
Python 3.9 API compatibility pass; successful runs on CPython 3.12 can hide this.

For final submission checks, use NWERC's compiler flags and Python runtime:

```sh
g++ -x c++ -std=gnu++20 -Wall -O2 -static -pipe -DONLINE_JUDGE -DDOMJUDGE -o A/A A/cpp.cpp
env ONLINE_JUDGE=1 DOMJUDGE=1 pypy3 A/py.py < A/1.in
```

Keep the short local debugging commands from the setup guide as well. The judge's
2 GiB limit applies to each submission including its runtime, not to the desktop
VM. VM timings also depend on host load; use the separate judgehost for judged
time/memory verdicts.

## Desktop access and clean rehearsals

Start with the browser console. For regular practice, use Proxmox's SPICE console
with a Virt-Viewer client on Windows or Linux. `spice-vdagent` in the guest helps
desktop integration; QEMU Guest Agent is a separate, optional management aid.
Clipboard/file sharing is convenient for configuration, but leave it unused when
rehearsing setup from the printed notebook. Validate keyboard symbols, terminal
shortcuts and the intended display resolution through the chosen console.

The public image documents a `team` account with an empty password and sudo.
Keep it on a private network, set appropriate credentials, and create/use an
unprivileged practice account for rehearsals. Leave guest SSH/RDP off unless
needed; Proxmox's console works without enabling either in the guest. This also
avoids changing the image's default remote-login restrictions.

After checking the desktop, tools, persistence and sample programs:

1. Shut down the guest and save a clean snapshot or turn the verified base into
   a template. All VM disks, including EFI, must support the chosen snapshot path.
   Keep a backup too.
2. Make a full clone for each independent practice workstation. Linked clones
   are an option when the storage supports them, but depend on the base template.
   If several clones run together, check unique guest identities/hostnames and
   DHCP leases as well as their virtual MAC addresses.
3. Keep `clean-start` before any contest folders, shell functions or personal
   editor settings are added. Rehearse manually creating `tmpl/py.py`,
   `tmpl/cpp.cpp` and the commands from the printed guide. Do not auto-source the
   repo's setup script in this exercise.
4. Reboot once, verify files persist, and test terminal input, sample files and
   `tp`/`tc` in both languages. Export solutions before reverting a practice clone.
5. For a mock contest, allow access to the practice DOMjudge endpoint and any
   intended local documentation. Restrict general Internet access at the VM's
   network/firewall while keeping the Proxmox management console reachable.

The baseline is ready only after these checks pass. The original compressed
download and its complete decompressed contents have been verified locally;
the user is operating Proxmox. A working desktop from the corrected source has
not yet been observed.
