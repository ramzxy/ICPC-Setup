# DOMjudge: running practice competitions

Open [DOMjudge over Tailscale](http://100.103.219.70:30080) or
[on the LAN](http://192.168.1.117:30080), and log in as **admin**.
The saved initial password is on the VM: `sudo cat /root/domjudge-admin-password.txt`.
Manage competitions in the website; the deployment stays running. Participants
need LAN/Tailscale access and a DOMjudge login.

## 1. Create a competition

Go to **Contests → Add**, give it a name and unique ID/short name (for example
`nwerc2025-round2`), and set its milestones:

| Setting | Five-hour competition | Always-open practice |
| --- | --- | --- |
| Activate time | Before the start, so teams can select the contest | A time in the past |
| Start time | Your chosen date and time | A time in the past |
| Start time enabled | Yes | Yes |
| End time | `+5:00:00` (relative to start) | A distant date, such as `2099-12-31 23:59:59 Europe/Amsterdam` |
| Freeze / unfreeze | Leave blank for an ordinary practice contest | Leave blank |
| Deactivate time | Leave blank to keep results visible afterward | Leave blank |

Enter your start date as `YYYY-MM-DD HH:MM:SS Europe/Amsterdam`.
Set **Enable contest → Yes**. Choose **Open contest to all teams → Yes**, or
choose **No** and select the participating teams/categories. Save: it starts
automatically at its start time. To delay it, disable the start time, choose a
new start time, then enable it again.

**Rerunning NWERC 2025?** Create a fresh contest and use **Contests → Edit →
Problems** to add the existing A–L problems. Scores start fresh; open-practice
progress is preserved. Shared problems also share tests and judging settings.

## 2. Load a new set from a ZIP

For a bundle like the supplied `solutions.zip`:

1. Extract the **outer** ZIP. Keep inner problem ZIPs intact. Choose the
   **legacy / DOMjudge-compatible** release when available. Do not distribute
   jury code or secret tests to participants.
2. Create the contest as above. The old `contest.yaml` has historical dates;
   setting up a fresh contest manually is usually easiest.
3. In **Import / Export → Problems → Import JSON / YAML**, select your new
   contest and import `problems.yaml`. This creates the problem entries with
   their letters, colors and time limits. Do this once, before the ZIP uploads.
4. Under **Problems**, open each newly created problem and choose **Edit**.
   In the **Problem archive** form, choose its matching ZIP
   (`arcadecrane.zip`, etc.), select your contest, and click **Import**.
   This fills that existing entry with its statement, tests and validators.
5. Check import messages, A–L ordering, statements, testcase counts and limits.
   Run **Config checker** and confirm the worker is enabled under **Judgehosts**.

The outer bundle and `samples.zip` are not individual problem packages. If there
is no `problems.yaml`, use **Import / Export → Problems → Import archive** for
each problem ZIP, selecting the contest, then check its letter and time limit.
To update an existing problem, use its Edit/upload form; repeatedly importing
`problems.yaml` creates new records. ZIP updates can append tests, so inspect the
result and back up before replacing an existing set.

Before starting, test jury solutions using an admin/jury account linked to the
hidden **Jury verification** team (Users → Edit → Team). Include that team in
restricted contests. **Judging verifier** compares expected and actual verdicts.
Jury tests can run after activation, before the start. The installed 2025 set is
already checked.

## 3. Create teams and logins

1. **Teams → Add**: enter a team name and choose the visible **Self-Registered**
   category. That category name does not enable public signup.
2. Select **Add user to this team → Create new user**, enter a username, and save.
3. **Users → Generate passwords → Teams without password** downloads
   `accounts.tsv`. Give each participant their own row privately. Selecting
   **All teams** also resets existing team passwords.

A **Team** owns scores; a **User** owns credentials. Use one team per person for
separate scores. For shared progress, share a login or link multiple Users with
the **Team member** role to one Team. Participants need no Linux accounts or
admin/jury roles. Include their team/category if the contest restricts participation.

## 4. Run and finish it

Monitor **Submissions**, **Scoreboard**, **Clarifications** and **Judgehosts**.
Check the participant view with a team login. After changing tests, validators or
compiler settings, rejudge affected submissions; old verdicts do not update themselves.

After the end time, submissions can still be judged but are marked too late and
do not count toward scores. Leave Deactivate blank to retain access to results.
If you used a scoreboard freeze, release it with the contest's unfreeze control.
To hide a contest and stop its judging, set **Enable contest → No**.
For another attempt, create another contest instead of deleting old submissions.

## 5. Basic server management

New contests do not require restarting or reinstalling anything. On the k3s VM:

```sh
k() { sudo k3s kubectl "$@"; }
k -n domjudge get pods,pvc
k -n domjudge logs deployment/domjudge-judgehost --tail=50
k -n domjudge logs deployment/domjudge-domserver --tail=50
```

All three pods should be Ready. For queued submissions, check the worker is enabled
and healthy; for internal judging errors, read its logs. Restart a stuck worker
only when needed: `k -n domjudge rollout restart deployment/domjudge-judgehost`.
Keep the database PVC and its password Secret. Before large imports or upgrades,
save a private SQL backup and copy it off the VM; existing backups and deployment
details are in [the deployment record](domjudge-deployment.md).

For infrastructure changes, edit the Helm chart / active `HelmChartConfig`.
The release is managed by k3s's Helm controller; see [the chart guide](../deploy/charts/domjudge/README.md).

References: [DOMjudge administration](https://www.domjudge.org/docs/manual/9.0/config-basic.html),
[ZIP format](https://www.domjudge.org/docs/manual/9.0/problem-format.html).
For the currently installed set, see [NWERC 2025 practice](nwerc2025-practice.md).
