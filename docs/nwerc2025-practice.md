# NWERC 2025 open practice

Website: `http://100.103.219.70:30080` over the existing Tailscale connection,
or `http://192.168.1.117:30080` on the LAN. A DOMjudge login does not grant
Tailscale access; participants need access to one of those networks separately.

Select **NWERC 2025 - Open Practice** in DOMjudge's contest selector.
Its API ID is `nwerc2025-practice`. It started on 11 November 2026 and has an end
time of **1 January 2100, 00:59:59 Europe/Amsterdam** (31 December 2099, 23:59:59 UTC).
There is no scoreboard freeze or deactivate time. DOMjudge requires an end time;
the distant date makes this effectively permanent practice without a five-hour
deadline. You can change these milestones later under Contests → Edit.

## Imported problems

The original uploaded `solutions.zip` was no longer in Downloads, so the server
downloaded the official [DOMjudge-compatible legacy release](https://2025.nwerc.eu/nwerc2025-problems-legacy.zip)
linked on [the NWERC 2025 website](https://2025.nwerc.eu/).
Archive SHA256:
`71e35ec9de399bedde4b43e5d0a441f83c30f5a356b8156dc6c609e184c15cb7`.
The archive and jury code are kept privately under `/root/nwerc2025` on the VM,
not in this Git repository or participant solution directories.

All twelve problem packages were imported with their official PDFs, sample and
secret tests, output validators and time limits. The database/API testcase counts
were checked against the ZIPs: **1,431 cases** in total.

| Label | Problem | Time limit | Cases |
| --- | --- | --- | --- |
| A | Arcade Crane | 2 s | 94 |
| B | Bisecting Bargain | 2 s | 212 |
| C | Canal Crossing | 4 s | 82 |
| D | Dreamcatcher | 1 s | 88 |
| E | Erratic Lights | 2 s | 178 |
| F | Fair Share | 3 s | 101 |
| G | Group Photo | 4 s | 146 |
| H | Hasty Haul | 3 s | 68 |
| I | Illuminated Stalls | 8 s | 150 |
| J | Juggling Keys | 5 s | 102 |
| K | KIT Finding | 2 s | 139 |
| L | Last Christmas | 1 s | 71 |

The 2 GiB global memory limit applies unless a problem overrides it. Hardware
timings are still those of the practice VM. Hasty Haul's multi-pass flag and
three-pass limit were preserved; no validator was replaced with plain output
comparison. Hasty Haul passed accepted C++ and Python jury submissions, and an
intentionally wrong program received WA. The C++ sandbox recorded 68 first-pass,
63 second-pass and 24 third-pass executions, confirming real multi-pass judging.
Jury verification is recorded in `/root/nwerc2025/import-state.json` on the VM.

All **15** final jury checks passed: one original accepted C++ solution for each
of the twelve problems, accepted Python solutions for H and L, and the expected
WA for a deliberately invalid H submission. These ran against the full imported
test sets, not just sample data. This verifies this particular legacy archive and
installed backend; it does not prove every future validator or jury variant.

## Providing logins

Linux accounts, SSH and sudo are not needed for participants. DOMjudge separates
**users** (login credentials) from **teams** (submissions and scoreboard progress).

To create a normal practice login as administrator:

1. Open **Teams → Add**, enter a team name, and use the **Self-Registered** team
   category (the visible participant category; its name does not enable signup).
2. Under **Add user to this team**, select **Create new user**, enter a username,
   then save.
3. Open **Users → Generate passwords** and select **Teams without password**.
   This downloads `accounts.tsv` with the new usernames/passwords. Share only each
   participant's own credentials privately. This option leaves existing passwords
   alone; **All teams** regenerates existing team passwords too.

For the three-person team, either share one team login or create three Users
with the **Team member** role and link them to the same Team. Both approaches
share submissions/progress. To practice against each other, create a separate
Team for each person, each with its own user. Participant accounts do not need
admin, jury or judgehost roles.

If you prefer signup without issuing credentials yourself, edit the participant
category under **Team categories** and enable **Allow self-registration**. A
registration link then appears on the login page. Self-registration is currently
disabled; it is not required for the administrator-created accounts above.

Official account/registration documentation:
[users and teams](https://www.domjudge.org/docs/manual/9.0/config-basic.html#setting-up-users-and-teams),
[self-registration](https://www.domjudge.org/docs/manual/9.0/config-advanced.html#self-registration).

## Administration

- The hidden **Jury verification** team is in the System category. Reference
  submissions are excluded from the participant scoreboard; do not share their
  source or secret data during blind practice.
- The installed language setup is GNU++20 / GCC 13.3.0 and PyPy 7.3.15 /
  Python 3.9.18, as recorded in [the deployment notes](domjudge-deployment.md).
- `/root/nwerc2025/pre-import.sql.gz` is the database backup from immediately
  before the import; `/root/nwerc2025/verified-practice.sql.gz` is the post-import
  backup after all jury checks passed. Both are root-only VM-local files, not
  off-machine disaster-recovery backups. Keep the original database Secret with
  any retained PVC.
- Do not re-import `problems.yaml` into this existing contest: that endpoint
  creates new problem records. Use the existing problem's ZIP-update operation
  if a package needs replacing. Import state prevents accidental duplicate
  uploads in the one-off server helper.
- Public router forwarding and HTTPS remain unconfigured, as requested.
