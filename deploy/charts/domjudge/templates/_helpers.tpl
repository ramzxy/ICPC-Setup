{{- define "domjudge.fullname" -}}
{{- default .Release.Name .Values.fullnameOverride | trunc 40 | trimSuffix "-" -}}
{{- end -}}
{{- define "domjudge.labels" -}}
app.kubernetes.io/name: domjudge
app.kubernetes.io/instance: {{ .Release.Name | quote }}
app.kubernetes.io/managed-by: {{ .Release.Service | quote }}
helm.sh/chart: {{ printf "%s-%s" .Chart.Name .Chart.Version | replace "+" "_" | quote }}
{{- end -}}
{{- define "domjudge.selector" -}}
app.kubernetes.io/name: domjudge
app.kubernetes.io/instance: {{ .Release.Name | quote }}
{{- end -}}
{{- define "domjudge.databaseSecret" -}}
{{- default (printf "%s-database" (include "domjudge.fullname" .)) .Values.database.existingSecret -}}
{{- end -}}
{{- define "domjudge.judgehostSecret" -}}
{{- default (printf "%s-judgehost" (include "domjudge.fullname" .)) .Values.judgehost.existingSecret -}}
{{- end -}}
