# Archivo de idioma Español.
#
# Services esta protegido por derechos de autor (c) 1996-1999 Andy Church
#     E-mail: <achurch@dragonfire.net>
# Este programa es gratis pero esta registrado; vea el archivo COPYING para
# detalles.
# Traducción por:
# José R. Holzmann "Joselito" joselito@sandnet.org
# Raúl S. Villarreal "CheMaN" cheman@sandnet.org

# When translating this file to another language, keep in mind that the
# order of parameters for sprintf() is fixed in the source code, so your
# messages need to take the same parameters in the same order as the
# English messages do.  (Obviously, this doesn't hold for the strftime()
# format lines immediately below.)  If you can't get a natural translation
# of a message without changing the order of the parameters, let me know
# (achurch@dragonfire.net) which message is causing a problem and I'll see
# what I can do.
#
# In help messages, "%S" (capital S, not lowercase) refers to the name of
# the service sending the message; for example, in NickServ help messages,
# "%S" is replaced by "NickServ" (or whatever it is renamed to in
# config.h).  The %S's do not count as sprintf() parameters, so they can be
# rearranged, removed, or added as necessary.

###########################################################################
#
# Nombre de este idioma
#
###########################################################################

# For languages other than English, this string should have the following
# format:
#     language-name-in-language (language-name-in-English)
# For example, "Español (Spanish)" or "Français (French)".

LANG_NAME
	Castellano (Spanish)

###########################################################################
#
# Mensajes Generales
#
###########################################################################

# strftime() format strings.  man 3 strftime for information on the
# meanings of the format specifiers.  Short version:
#	%a = weekday name (short)	%H = hour
#	%b = month name (short)		%M = minute
#	%d = day of month		%S = second
#	%Y = year			%Z = time zone

# This is used to display a date and time together.
STRFTIME_DATE_TIME_FORMAT
	%H:%M:%S %d/%B/%Y %Z
# This is used as the format string for strftime() for a date and time
# format (English: including weekday).
STRFTIME_LONG_DATE_FORMAT
	%a %b %d %Y
STRFTIME_SHORT_DATE_FORMAT
# This is used as the format string for strftime() for a date alone in
# short format (English: without weekday).
	%b %d %Y

# These tell our strftime() what the names of months and days are.  If you
# don't use %a, %A, %b, or %B in your strftime() strings above, you can
# leave these empty.  However, if you enter names, they MUST stay in order,
# one per line, and the list MUST be complete!

# %a
STRFTIME_DAYS_SHORT
	Dom
	Lun
	Mar
	Mie
	Jue
	Vie
	Sab
# %A
STRFTIME_DAYS_LONG
	Domingo
	Lunes
	Martes
	Miércoles
	Jueves
	Viernes
	Sábado
# %b
STRFTIME_MONTHS_SHORT
	Ene
	Feb
	Mar
	Abr
	May
	Jun
	Jul
	Ago
	Sep
	Oct
	Nov
	Dic
# %B
STRFTIME_MONTHS_LONG
	Enero
	Febrero
	Marzo
	Abril
	Mayo
	Junio
	Julio
	Agosto
	Septiembre
	Octubre
	Noviembre
	Diciembre

# This is used in ChanServ/NickServ INFO displays.
COMMA_SPACE
	,

# Various error messages.
USER_RECORD_NOT_FOUND
	Error Interno,imposible procesar comando.
UNKNOWN_COMMAND
	Comando desconocido 12%s.
UNKNOWN_COMMAND_HELP
	Comando desconocido 12%s. "/msg %s HELP" para ayuda.
SYNTAX_ERROR
	Sintaxis: 12%s
MORE_INFO
	12/msg %s HELP %s para mas información.
NO_HELP_AVAILABLE
	No dispongo de ayuda para 12%s.

BAD_USERHOST_MASK
	La máscara debe estar en formato 12usuario@12host.

BAD_EXPIRY_TIME
	Tiempo de expiración invalido.

SERVICE_OFFLINE
	12%s esta actualmente fuera de servicio.
READ_ONLY_MODE
	4Advertencia:12 Los servicios están en modo de sólo lectura; los cambios no serán grabados!
ACCESS_DENIED
	4Acceso denegado.
PERMISSION_DENIED
	4Permiso denegado.
PASSWORD_INCORRECT
	4Contraseña incorrecta.

MORE_OBSCURE_PASSWORD
	Intente de nuevo con una contraseña mas difícil. Las contraseñas deben ser de 5 caracteres mínimo,no debe ser algo fácil de descifrar (ejemplo. su verdadero nombre o su nick), tampoco deben contener espacios.
PASSWORD_TRUNCATED
	4Atención: Su contraseña ha sido limitada a 12%d caracteres.

NICK_NOT_REGISTERED
	Tu nick no esta registrado.
NICK_NOT_REGISTERED_HELP
	Tu nick no esta registrado. Escribe 12/msg %s HELP12 para obtener informacion sobre como registrar su nick.
NICK_SUSPENDED
	Este nick ha sido 12SUDPENDido temporalmente.
	Motivo: %s
NICK_IS_SUSPENDED
	Tu nick 12%s ha sido 12SUSPENDido temporalmente.
	Causa de suspensión: %s
NICK_IS_REACTIVED
	Tu nick 12%s ha sido reactivado.
	Vuelve a identificar con tu nick.
NICK_X_NOT_REGISTERED
	El nick 12%s no esta registrado.
NICK_X_NOT_IN_USE
	El nick 12%s no esta en uso en este momento.
NICK_X_FORBIDDEN
	El nick 12%s no puede ser registrado o utilizado.
NICK_X_SUSPENDED
	El nick 12%s ha sido suspendido temporalmente.
CHAN_X_NOT_REGISTERED
	El canal 12%s no esta registrado.
CHAN_X_NOT_IN_USE
	El canal 12%s no existe.
CHAN_X_FORBIDDEN
	El canal 12%s no puede ser registrado o utilizado.
CHAN_X_SUSPENDED
	El canal 12%s está 12SUSPENDido temporalmente.
NICK_IDENTIFY_REQUIRED
	Contraseña es requerida para ese comando.
	Vuelva a intentar después de escribir 12/msg %s IDENTIFY contraseña.

CHAN_IDENTIFY_REQUIRED
	La contraseña es necesaria para ese comando.
	Vuelve a intentar después de escribir 12/msg %s IDENTIFY contraseña.

###########################################################################
#
# Mensajes de Nickserv
#
###########################################################################

# Automatic messages
NICK_IS_REGISTERED
	4Este nick es pertenece a otra persona. Escoge otro nick.
	Si es el tuyo, escribe 12/msg %s IDENTIFY contraseña.
NICK_IS_SECURE
	Este nick esta registrado y protegido. Si es su nick, escriba 12/msg %s IDENTIFY contraseña. Si no lo es, escoge un nick diferente.
NICK_MAY_NOT_BE_USED
	Este nick no se puede utilizar. Debes escoger otro nick.
DISCONNECT_IN_1_MINUTE
	4Si no cambias tu nick antes de un minuto, serás desconectado.
DISCONNECT_IN_20_SECONDS
	12Si no cambias tu nick antes de 20 segundos, serás desconectado.
DISCONNECT_NOW
	Este nick ha sido registrado; no lo puedes usar.
	Escoge otro nick antes de reconectar.
FORCENICKCHANGE_IN_1_MINUTE
	4Si no cambias tu nick antes de un minuto, será cambiado por otro aletorio
FORCENICKCHANGE_IN_20_SECONDS
	12Si no cambias tu nick antes de 20 segundos, será cambiado por otro aleatorio.
FORCENICKCHANGE_NOW
	Este nick ha sido registrado; no lo puedes usar.
	Tu nick ha sido cambiado a 12%s.

# REGISTER responses
NICK_REGISTER_SYNTAX
	12REGISTER <direccion@correo.electronico>
NICK_REGISTRATION_DISABLED
	El registro de nicks esta temporalmente deshabilitado.
NICK_REGISTRATION_FAILED
	El registro ha fallado.
NICK_REG_PLEASE_WAIT
	Debe esperar 12%d segundos antes de volver a utilizar el comando 12REGISTER.
NICK_CANNOT_BE_REGISTERED
	El nick 12%s no puede ser registrado.
NICK_ALREADY_REGISTERED
	El nick 12%s ya esta registrado!
NICK_MAIL_INVALID
	Dirección Email invalida, utiliza tu mail REAL!!
NICK_MAIL_ANONYMOUS
	Dominio de correo no válido.
	No utilices servicios anonimos.
NICK_MAIL_ABUSE
	No puedes registrar mas de %d nicks por email.
NICK_REGISTERED
	El nick 12%s esta registrado bajo tu cuenta: 12%s
NICK_PASSWORD_IS
	Tu contraseña es 12%s, recuerdala para uso posterior.
NICK_IN_MAIL
	Revisa tu correo en unos minutos, te llegará una password temporal.

# IDENTIFY responses
NICK_IDENTIFY_SYNTAX
	12IDENTIFY <password>
NICK_IDENTIFY_FAILED
	4Contraseña incorrecta.
NICK_IDENTIFY_SUCCEEDED
	3Contraseña aceptada. Bienvenid@ a casa ;)
NICK_IDENTIFY_X_MODE_R
	3Contraseña aceptada. Bienvenid@ a casa ;)12 [identificado por +r]
NICK_IS_IDENTIFIED
	3Ya estabas identificad@ ;)

# DROP responses
NICK_DROP_SYNTAX
	12DROP [nick]
NICK_DROP_DISABLED
	La cancelación de nicks esta temporalmente deshabilitada.
NICK_DROP_FORBIDDEN
	No se puede dropar un nick forbideado
NICK_DROPPED
	Tu nick ha sido cancelado.
NICK_X_DROPPED
	El nick 12%s ha sido cancelado.

# SET responses
NICK_SET_SYNTAX
	12SET <opción> <parámetros>
NICK_SET_SERVADMIN_SYNTAX
	12SET [nick] <opción> <parámetros>
NICK_SET_DISABLED
	La configuración de opciones de nick esta temporalmente deshabilitada.
NICK_SET_UNKNOWN_OPTION
	4Opción SET12 %s desconocida.
NICK_SET_UNKNOWN_OPTION_OR_BAD_NICK
	4Opción SET 12%s desconocida, o nick no registrado.

# SET PASSWORD responses
NICK_SET_PASSWORD_FAILED
	La contraseña no se pudo cambiar.
NICK_SET_PASSWORD_CHANGED
	Contraseña cambiada.
NICK_SET_PASSWORD_CHANGED_TO
	Contraseña cambiada a 12%s.

# SET URL responses
NICK_SET_URL_CHANGED
	URL cambiada a %s.
NICK_SET_URL_UNSET
	URL deshabilitada.

# SET EMAIL responses
NICK_SET_EMAIL_CHANGED
	Dirección de E-mail cambiada a 12%s.
NICK_SET_EMAIL_UNSET
	Dirección de E-mail deshabilitada.

# SET LANGUAGE responses
NICK_SET_LANGUAGE_SYNTAX
	12SET LANGUAGE <número>
NICK_SET_LANGUAGE_UNKNOWN
	Numero %d de lenguaje desconocido.  Escriba 12/msg %s HELP SET LANGUAGE para ver una lista de lenguajes disponibles.
NICK_SET_LANGUAGE_CHANGED
	Lenguaje cambiado a 12Castellano.

# SET KILL responses
NICK_SET_KILL_SYNTAX
	12SET KILL {ON | QUICK | OFF}
NICK_SET_KILL_IMMED_SYNTAX
	12SET KILL {ON | QUICK | IMMED | OFF}
NICK_SET_KILL_ON
	Protección de KILL 12activada.
NICK_SET_KILL_QUICK
	Protección de KILL 12activada, con una espera reducida.
NICK_SET_KILL_IMMED
	Protección de KILL 12activada, sin espera.
NICK_SET_KILL_IMMED_DISABLED
	La opción 12IMMED no esta disponible en esta red.
NICK_SET_KILL_OFF
	Protección de KILL 12desactivada.

# SET SECURE responses
NICK_SET_SECURE_SYNTAX
	12SET SECURE {ON | OFF}
NICK_SET_SECURE_ON
	Opción SECURE 12activada.
NICK_SET_SECURE_OFF
	Opción SECURE 12desactivada.

# SET PRIVATE responses
NICK_SET_PRIVATE_SYNTAX
	12SET PRIVATE {ON | OFF}
NICK_SET_PRIVATE_ON
	Opción PRIVATE 12activada.
NICK_SET_PRIVATE_OFF
	Opción PRIVATE 12desactivada.

# SET HIDE responses
NICK_SET_HIDE_SYNTAX
	12SET HIDE {EMAIL | USERMASK | QUIT} {ON | OFF}
NICK_SET_HIDE_EMAIL_ON
	Tu dirección de E-mail no se mostrará en 12%s INFO.
NICK_SET_HIDE_EMAIL_OFF
	Tu dirección de E-mail se mostrará en 12%s INFO.
NICK_SET_HIDE_MASK_ON
	Tu ultima mascara de usuario@host no se mostrará en 12%s INFO.
NICK_SET_HIDE_MASK_OFF
	Tu ultima mascara de usuario@host se mostrará en 12%s INFO.
NICK_SET_HIDE_QUIT_ON
	Tu ultimo mensaje de quit no se mostrará en 12%s INFO.
NICK_SET_HIDE_QUIT_OFF
	Tu ultimo mensaje de se mostrará en 12%s INFO.

NICK_SET_VHOST_SYNTAX
	12SET [nick] IPVIRTUAL {<máscara> | OFF}
NICK_SET_VHOST_ON
	IPvirtual cambiada a 12%s
NICK_SET_VHOST_OFF
	IPvirtual deshabilitada.

NICK_MUST_BE_ON_BDD
	El nick debe estar en la BDD para poder utilizar esto.

# SET BDD responses
NICK_SET_BDD_SYNTAX
	12SET [nick] BDD {ON | OFF}
NICK_SET_BDD_ON
	El nick 12%s ha sido migrado a BDD
NICK_SET_BDD_OFF
	El nick 12%s ha salido de la BDD

NICK_BDD_NEW_REG
	Su clave es 12%s, recuérdela por si no le llega confirmación al correo. Utilice 12/nick %s:%s para identificarse.

# SET NOEXPIRE responses
NICK_SET_NOEXPIRE_SYNTAX
	12SET [nick] NOEXPIRE {ON | OFF}
NICK_SET_NOEXPIRE_ON
	El nick 12%s no expirará.
NICK_SET_NOEXPIRE_OFF
	El nick 12%s si expirará.

NICK_INFO_SERV_OPER
	Es un 12OPERador de los servicios de RED
NICK_INFO_SERV_ADMIN
	Es un 12ADMINistrador de los servicios de RED
NICK_INFO_SERV_ROOT
	Es el 12ROOT de la RED

NICK_DO_INFO_OVER_OPER
	12%s ha utilizado %s INFO sobre ti

# ACCESS responses
NICK_ACCESS_SYNTAX
	12ACCESS {ADD | DEL | LIST} [mascara]
NICK_ACCESS_ALREADY_PRESENT
	La máscara 12%s ya existe en su lista de acceso.
NICK_ACCESS_REACHED_LIMIT
	Sólo puedes tener 12%d entradas en el access para tu nick.
NICK_ACCESS_ADDED
	12%s agregada a tu lista de acceso.
NICK_ACCESS_NOT_FOUND
	12%s no encontrada en tu lista de acceso.
NICK_ACCESS_DELETED
	12%s borrada de yu lista de acceso.
NICK_ACCESS_LIST
	Lista de acceso:
NICK_ACCESS_LIST_X
	Lista de acceso para 12%s:

# LINK responses
NICK_LINK_SYNTAX
	12LINK <nick> <password>
NICK_LINK_DISABLED
	EL comando 12LINK no está disponible.
NICK_LINK_FAILED
	Lo siento, el link ha fallado.
NICK_NO_LINK_SAME
	No puede linkar su nick actual!
NICK_LINK_CIRCULAR
	Imposible linkar a nick 12%s: link circular detectado.
NICK_LINKED
	Tu nick ha sido linkado a 12%s.

# UNLINK responses
NICK_UNLINK_SYNTAX
	12UNLINK [nick password]
NICK_UNLINK_FAILED
	Lo siento, el deslink ha fallado.
NICK_NOT_LINKED
	Tu nick no esta linkado.
NICK_UNLINKED
	Tu nick ha sido deslinkado de 12%s.
NICK_X_NOT_LINKED
	El nick 12%s no esta linkado.
NICK_X_UNLINKED
	El nick 12%s ha sido deslinkado de 12%s.

# LISTLINKS responses
NICK_LISTLINKS_SYNTAX
	12LISTLINKS <nick> [ALL]
NICK_LISTLINKS_HEADER
	Lista de nicks linkados a 12%s:
NICK_LISTLINKS_FOOTER
	12%d nicks linkados.

# INFO responses
NICK_INFO_SYNTAX
	12INFO nick [ALL]

NICK_INFO_LINKED
	Nick REAL (linkado a): %s
NICK_INFO_SUSPENDED
	SUSPENDIDO: %s
NICK_INFO_REALNAME
	RealName de 12%s es 12%s
NICK_INFO_ADDRESS
	Ultima dirección: 12%s
NICK_INFO_ADDRESS_ONLINE
	Esta on-line desde: 12%s
NICK_INFO_ADDRESS_ONLINE_NOHOST
	Esta on-line, mas información escriba 12/WHOIS %s.
NICK_INFO_TIME_REGGED
	Hora de registro: 12%s
NICK_INFO_LAST_SEEN
	Ultima hora por la red: 12%s
NICK_INFO_LAST_QUIT
	Ultimo mensaje de salida: 12%s
NICK_INFO_URL
	URL: 12%s
NICK_INFO_EMAIL
	E-mail: 12%s
NICK_INFO_OPTIONS
	Opciones: %s
# These strings MUST NOT be empty
NICK_INFO_OPT_KILL
	12Protección de KILL
NICK_INFO_OPT_SECURE
	12Seguro
NICK_INFO_OPT_PRIVATE
	12Privado
NICK_INFO_OPT_NONE
	12Ninguna

NICK_INFO_OPT_BDD
	12En BDD

NICK_INFO_NO_EXPIRE
	Expiración del nick: 12no expirará
NICK_INFO_LINKS
	Linkado por 12%s (12%s)

# LIST responses
NICK_LIST_SYNTAX
	12LIST <patron>
NICK_LIST_SERVADMIN_SYNTAX
	12LIST <patron> [FORBID] [NOEXPIRE] [SUSPEND]
NICK_LIST_HEADER
	Lista de entradas que coinciden con 12%s:
NICK_LIST_HIDDEN
	    %-20s [Oculto]
NICK_LIST_FORBIDDEN
	    %-20s [Forbideado]
NICK_LIST_SUSPENDED
	    %-20s [Suspendido]
NICK_LIST_NORMAL
	    %-20s  %s
NICK_LIST_RESULTS
	Fin de lista - 12%d/12%d entradas mostradas.

# RECOVER responses
NICK_RECOVER_SYNTAX
	12RECOVER <nick> [password]
NICK_NO_RECOVER_SELF
	No puede recuperar su propio nick!
NICK_RECOVERED
	Usuario utilizando su nick ha sido desconectado.
	12/msg %s RELEASE %s para recuperar antes de un minuto.

# RELEASE responses
NICK_RELEASE_SYNTAX
	12RELEASE <nick> [password]
NICK_RELEASE_NOT_HELD
	Nick 12%s no esta siendo protegido en este momento.
NICK_RELEASED
	La protección de Services en su nick ha sido liberada.
# USERIP
NICK_USERIP_SYNTAX
	12USERIP <nick>
NICK_USERIP_CHECK_OK
	El nick 12%s está conectando desde: 12%s (12%s)
NICK_USERIP_CHECK_NO
	El nick 12%s no está conectado en este momento.

# GHOST responses
NICK_GHOST_SYNTAX
	12GHOST <nick> [password]
NICK_NO_GHOST_SELF
	No puedes hacerte ghost a ti mismo!
NICK_GHOST_KILLED
	Sesión fantasma de tu nick ha sido desconectada.

# STATUS responses
NICK_STATUS_OFFLINE
	STATUS %s 0 (No conectado)
NICK_STATUS_NOT_REGISTRED
	STATUS %s 0 (Conectado, no registrado)
NICK_STATUS_NOT_IDENTIFIED
	STATUS %s 1 (Conectado, no coincide mascara, ni identificado)
NICK_STATUS_RECOGNIZED
	STATUS %s 2 (Conectado, reconocido por máscara)
NICK_STATUS_IDENTIFIED
	STATUS %s 3 (Identificado)
NICK_STATUS_SUSPENDED
	STATUS %s 4 (Suspendido)

NICK_STATUS_ID_BDD
	Identificado por BDD

# SENDPASS responses
NICK_SENDPASS_SYNTAX
	12SENDPASS <nick>
NICK_SENDPASS_SUCCEEDED
	El password de 12%s ha sido enviado al mail: 12%s.
	Que revise su correo, en unos minutos le llegara el password.

# GETPASS responses
NICK_GETPASS_SYNTAX
	12GETPASS <nick>
NICK_GETPASS_UNAVAILABLE
	Comando 12GETPASS no disponible cuando encripción esta en uso.
NICK_GETPASS_PASSWORD_IS
	La contraseña de 12%s es 12%s.

NICK_IS_A_SERVICE
	12%s es un 12SERVICE de la red

# SUSPEND responses
NICK_SUSPEND_SYNTAX
	12SUSPEND <nick> <motivo>
NICK_SUSPEND_FORBIDDEN
	No se puede suspender un nick forbideado.
NICK_SUSPEND_SUSPENDED
	El nick 12%s ya estaba suspendido.
NICK_SUSPEND_OPER
	Que intentas:??
NICK_SUSPEND_SUCCEEDED
	El nick 12%s ha sido suspendido.
NICK_SUSPEND_FAILED
	No se puede suspender nick 12%s!

# UNSUSPEND responses
NICK_UNSUSPEND_SYNTAX
	12UNSUSPEND <nick>
NICK_SUSPEND_NOT_SUSPEND
	El nick 12%s no esta suspendido.
NICK_UNSUSPEND_SUCCEEDED
	El nick 12%s ha sido reactivado.
NICK_UNSUSPEND_FAILED
	No se puede reactivar el nick 12%s!


# FORBID responses
NICK_FORBID_SYNTAX
	12FORBID <nick> <motivo>
NICK_FORBID_FORBIDDEN
	El nick 12%s ya estaba forbideado.
NICK_FORBID_SUCCEEDED
	El nick 12%s esta prohibido.
NICK_FORBID_FAILED
	No se puede prohibir nick 12%s!

# UNFORBID responses
NICK_UNFORBID_SYNTAX
	12UNFORBID <nick>
NICK_UNFORBID_NOT_FORBID
	El nick 12%s no esta forbideado.
NICK_UNFORBID_SUCCEEDED
	El nick 12%s ha sido liberado.
NICK_UNFORBID_FAILED
	No se puede liberar nick 12%s!




###########################################################################
#
# Mensajes de ChanServ
#
###########################################################################

# Access level description
CHAN_LEVEL_AUTOOP
	Status de operador de canal automático
CHAN_LEVEL_AUTOVOICE
	Modo +v automático
CHAN_LEVEL_AUTODEOP
	No puede ser operador de canal
CHAN_LEVEL_AUTODEVOICE
	No puede ser moderador de canal
CHAN_LEVEL_NOJOIN
	No puede entrar a canal si esta en modo restringido (RESTRICTED)
CHAN_LEVEL_OPDEOP
	Permitido a usar comandos OP/DEOP
CHAN_LEVEL_VOICEDEVOICE
	Permitido a usar comandos VOICE/DEVOICE
CHAN_LEVEL_INVITE
	Permitido a usar comando INVITE
CHAN_LEVEL_KICK
	Permitido a usar comando KICK
CHAN_LEVEL_BAN
	Permitodo a usar comando BAN
CHAN_LEVEL_UNBAN
	Permitido a usar comando UNBAN
CHAN_LEVEL_AKICK
	Permitido a usar comando AKICK
CHAN_LEVEL_SET
	Permitido a usar comando SET (no siendo FUNDADOR)
CHAN_LEVEL_CLEAR
	Permitido a usar comando CLEAR
CHAN_LEVEL_RESET
	Permitido a usar comando RESET
CHAN_LEVEL_ACCESS_LIST
	Permitido a ver lista de acceso
CHAN_LEVEL_ACCESS_CHANGE
	Permitido a modificar lista de acceso
CHAN_LEVEL_MEMO
	Permitido a ver mensajes del canal

# Automatic responses
CHAN_IS_REGISTERED
	Este canal esta registrado con 12%s.
CHAN_NOT_ALLOWED_OP
	Usted no puede ser operador del canal 12%s.
CHAN_MAY_NOT_BE_USED
	4Este canal no puede ser utilizado.
CHAN_NOT_ALLOWED_TO_JOIN
	4Lo siento, no puedes permanecer en este canal.

# REGISTER responses
CHAN_REGISTER_SYNTAX
	12REGISTER <canal> <password> <descripcion>
CHAN_REGISTER_DISABLED
	Lo siento, registro de canal esta temporalmente desactivado.
CHAN_REGISTER_NOT_LOCAL
	Canales locales no pueden ser registrados.
CHAN_MUST_REGISTER_NICK
	Usted debe de registrar primero su nick. Escriba 12/msg %s HELP para obtener más información de como registrar nicks.
CHAN_MUST_IDENTIFY_NICK
	Por favor identifiquese primero con 12%s, utilizando el comando:
	12/msg %s IDENTIFY contraseña
CHAN_MAY_NOT_BE_REGISTERED
	El canal 12%s no puede ser registrado.
CHAN_ALREADY_REGISTERED
	El canal 12%s ya esta registrado!
CHAN_MUST_BE_CHANOP
	Debes ser operador de canal para poder registrarlo.
CHAN_REACHED_CHANNEL_LIMIT
	Lo siento, usted ya alcanzo su limite de 12%d canales.
CHAN_EXCEEDED_CHANNEL_LIMIT
	Lo siento, usted ya excedió su limite de 12%d canales.
CHAN_REGISTRATION_FAILED
	El registro ha fallado.
CHAN_REGISTERED
	El canal 12%s registrado bajo el nick: 12%s
CHAN_PASSWORD_IS
	La contraseña del canal es 12%s, recuerdala para uso posterior.

# IDENTIFY responses
CHAN_IDENTIFY_SYNTAX
	12IDENTIFY <canal> <password>
CHAN_IDENTIFY_FAILED
	4Contraseña incorrecta.
CHAN_IDENTIFY_SUCCEEDED
	Contraseña aceptada, eres reconocido con nivel de fundador de 12%s.

# DROP responses
CHAN_DROP_SYNTAX
	12DROP 12<canal> <motivo>
CHAN_DROP_DISABLED
	Lo siento, la cancelación de registro de canal esta temporalmente deshabilitada.
CHAN_DROPPED
	El canal 12%s ha sido cancelado.

# SET responses
CHAN_SET_SYNTAX
	12SET <canal> <opcion> <parametros>

CHAN_SET_DISABLED
	El ajuste de opciones de canal esta temporalmente deshabilitada.
CHAN_SET_UNKNOWN_OPTION
	4Opción SET 12%s desconocida.

# SET FOUNDER responses
CHAN_SET_FOUNDER_TOO_MANY_CHANS
	12%s tiene demasiados canales registrados.
CHAN_FOUNDER_CHANGED
	Fundador de 12%s cambiado a 12%s.

# SET SUCCESSOR responses
CHAN_SUCCESSOR_CHANGED
	Sucesor de 12%s cambiado a 12%s.
CHAN_SUCCESSOR_UNSET
	Sucesor parar 12%s 12deshabilitado.

# SET PASSWORD responses
CHAN_SET_PASSWORD_FAILED
	No se ha podido cambiar la contraseña.
CHAN_PASSWORD_CHANGED
	La contraseña de 12%s ha sido cambiada.
CHAN_PASSWORD_CHANGED_TO
	La contraseña de 12%s ha sido cambiada a 12%s.

# SET DESC responses
CHAN_DESC_CHANGED
	Descripción de 12%s cambiada a 12%s.

# SET URL responses
CHAN_URL_CHANGED
	URL de 12%s cambiada a 12%s.
CHAN_URL_UNSET
	URL de 12%s desactivada.

# SET EMAIL responses
CHAN_EMAIL_CHANGED
	Dirección de E-mail de 12%s cambiada a 12%s.
CHAN_EMAIL_UNSET
	Dirección de E-mail de 12%s desactivada.

# SET ENTRYMSG responses
CHAN_ENTRY_MSG_CHANGED
	Mensaje de entrada de 12%s cambiado.
CHAN_ENTRY_MSG_UNSET
	Mensaje de entrada de 12%s desactivado.

# SET TOPIC responses
CHAN_SET_TOPIC_FAILED
	No fue posible poner el tópic.

# SET MLOCK responses
CHAN_SET_MLOCK_KEY_REQUIRED
	Parámetro requerido para MLOCK +k.
CHAN_SET_MLOCK_LIMIT_REQUIRED
	Parámetro requerido para MLOCK +l.
CHAN_SET_MLOCK_LIMIT_POSITIVE
	Parámetro para MLOCK +l debe ser un número positivo.
CHAN_SET_MLOCK_UNKNOWN_CHAR
	Modo 12%c es desconocido e ignorado.
CHAN_MLOCK_CHANGED
	Candado de modo del canal 12%s cambiado a 12%s.
CHAN_MLOCK_REMOVED
	Candado de modo del canal 12%s eliminado.

# SET KEEPTOPIC responses
CHAN_SET_KEEPTOPIC_SYNTAX
	12SET <canal> KEEPTOPIC {ON | OFF}
CHAN_SET_KEEPTOPIC_ON
	Retención de tópico 12activada.
CHAN_SET_KEEPTOPIC_OFF
	Retención de tópico 12desactivada.

# SET TOPICLOCK responses
CHAN_SET_TOPICLOCK_SYNTAX
	12SET <canal> TOPICLOCK {ON | OFF}
CHAN_SET_TOPICLOCK_ON
	Candado de tópico 12activado.
CHAN_SET_TOPICLOCK_OFF
	Candado de tópico 12desactivado.

# SET PRIVATE responses
CHAN_SET_PRIVATE_SYNTAX
	12SET <canal> PRIVATE {ON | OFF}
CHAN_SET_PRIVATE_ON
	Opción de privacidad 12activada.
CHAN_SET_PRIVATE_OFF
	Opción de privacidad 12desactivada.
# SET SECUREOPS responses
CHAN_SET_SECUREOPS_SYNTAX
	12SET <canal> SECUREOPS {ON | OFF}
CHAN_SET_SECUREOPS_ON
	Opción de Secure ops 12activada.
CHAN_SET_SECUREOPS_OFF
	Opción de Secure ops 12desactivada.

# SET LEAVEOPS responses
CHAN_SET_LEAVEOPS_SYNTAX
	12SET <canal> LEAVEOPS {ON | OFF}
CHAN_SET_LEAVEOPS_ON
	Opción de Leave ops 12activada.
CHAN_SET_LEAVEOPS_OFF
	Opción de Leave ops 12desactivada.


# SET RESTRICTED responses
CHAN_SET_RESTRICTED_SYNTAX
	12SET <canal> RESTRICTED {ON | OFF}
CHAN_SET_RESTRICTED_ON
	Opción de acceso restringido 12activada.
CHAN_SET_RESTRICTED_OFF
	Opción de acceso restringido 12desactivada.

# SET SECURE responses
CHAN_SET_SECURE_SYNTAX
	12SET <canal> SECURE {ON | OFF}
CHAN_SET_SECURE_ON
	Opción Secure 12activada.
CHAN_SET_SECURE_OFF
	Opción Secure 12desactivada.

# SET OPNOTICE responses
CHAN_SET_OPNOTICE_SYNTAX
	12SET <canal> ISSUED {ON | OFF}
CHAN_SET_OPNOTICE_ON
	Opción Issued 12activada.
CHAN_SET_OPNOTICE_OFF
	Opción Issued 12desactivada.

# SET STAY responses
CHAN_SET_STAY_SYNTAX
	12SET <canal> STAY {ON | OFF}
CHAN_SET_STAY_ON
	Opción Stay 12activada.
CHAN_SET_STAY_OFF
	Opción Stay 12desactivada.

# SET NOEXPIRE responses
CHAN_SET_NOEXPIRE_SYNTAX
	SET <canal> NOEXPIRE {ON | OFF}
CHAN_SET_NOEXPIRE_ON
	Canal 12%s 4NO expirará.
CHAN_SET_NOEXPIRE_OFF
	Canal 12%s 4SI expirará.

# ACCESS messages
CHAN_ACCESS_SYNTAX
	12ACCESS <canal> {ADD|DEL|LIST} [nick [nivel] | entrada-lista]
CHAN_ACCESS_DISABLED
	La modificación a la lista de acceso de canal esta temporalmente deshabilitada.
CHAN_ACCESS_LEVEL_NONZERO
	El nivel de acceso debe ser diferente de cero.
CHAN_ACCESS_LEVEL_RANGE
	El nivel de acceso debe estar entre 12%d y 12%d.
CHAN_ACCESS_NICKS_ONLY
	La lista de acceso de canal debe contener solamente nicks registrados.
CHAN_ACCESS_REACHED_LIMIT
	Sólo puede haber 12%d entradas en el access en un canal.
CHAN_ACCESS_LEVEL_UNCHANGED
	EL nivel de acceso para 12%s en 12%s no ha cambiado de 12%d.
CHAN_ACCESS_LEVEL_CHANGED
	El nivel de acceso para 12%s en 12%s ha sido cambiado a 12%d.
CHAN_ACCESS_ADDED
	12%s ha sido agregado a lista de acceso de 12%s con nivel 12%d.
CHAN_ACCESS_NO_SUCH_ENTRY
	Entrada desconocida (12#%d) en la lista de acceso de 12%s.
CHAN_ACCESS_NOT_FOUND
	12%s no fue encontrado en la lista de acceso de 12%s.
CHAN_ACCESS_NO_MATCH
	No se han encontrado entradas en la lista de acceso de 12%s12.
CHAN_ACCESS_DELETED
	12%s borrado de la lista de acceso de 12%s.
CHAN_ACCESS_DELETED_ONE
	Borrada12 1 entrada de la lista de acceso de 12%s.
CHAN_ACCESS_DELETED_SEVERAL
	Borradas 12%s entradas de la lista de acceso de 12%s.
CHAN_ACCESS_LIST_HEADER
	Lista de acceso de 12%s:
	  Num   Niv  Nick
CHAN_ACCESS_LIST_EMPTY
	La lista de aceeso de 12%s está vacia.
CHAN_ACCESS_LIST_FORMAT
	  %3d  %4d  %s%s%s%s

# AOP/SOP error response
CHAN_NO_AOP_SOP
	Para asignar privilegios del canal a usuarios, usa el comando 12ACCESS.
	Escribe 12/msg %s HELP ACCESS para más información.

# AKICK responses
CHAN_AKICK_SYNTAX
	12AKICK <canal> {ADD | DEL | LIST | ENFORCE} [nick or mascara]
CHAN_AKICK_DISABLED
	Lo siento, la modificación de lista de autokicks de canal esta temporalmente deshabilitada.
CHAN_AKICK_ALREADY_EXISTS
	12%s ya existe en la lista de autorkicks de 12%s.
CHAN_AKICK_REACHED_LIMIT
	Lo siento, solo puede tener 12%d mascaras de autokick en un canal.
CHAN_AKICK_ADDED
	12%s agregado a la lista de autokick de 12%s.
CHAN_AKICK_NO_SUCH_ENTRY
	Entrada desconocida (12#%d) en la lista de autokick de 12%s.
CHAN_AKICK_NOT_FOUND
	12%s no se encuentra en la lista de autokick de 12%s.
CHAN_AKICK_DELETED
	12%s borrado de la lista de autokick de 12%s.
CHAN_AKICK_DELETED_ONE
	Borrada12 1 entrada en la lista de autokick de 12%s.
CHAN_AKICK_DELETED_SEVERAL
	Borradas 12%s entradas en la lista de autokick de 12%s.
CHAN_AKICK_NO_MATCH
	No se encuentra entradas en la lista de autokick de 12%s.
CHAN_AKICK_LIST_EMPTY
	La lista de autokick de 12%s está vacia.

CHAN_AKICK_LIST_HEADER
	Lista de autokick de 12%s:
CHAN_AKICK_LIST_FORMAT
	  %3d %s%s
CHAN_AKICK_ENFORCE_DONE
	12AKICK ENFORCE para 12%s completado; 12%d usuarios son afectados.

# LEVELS responses
CHAN_LEVELS_SYNTAX
	12LEVELS <canal> {SET | DIS[ABLE] | LIST | RESET} [item [level]]
CHAN_LEVELS_RANGE
	El nivel debe de estar entre 12%d y 12%d.
CHAN_LEVELS_CHANGED
	El nivel para 12%s en canal 12%s cambiado a 12%d.
CHAN_LEVELS_UNKNOWN
	Opción 12%s desconocida.  Escriba 12/msg %s HELP LEVELS DESC para una lista de opciones validas.
CHAN_LEVELS_DISABLED
	12%s deshabilitado en canal 12%s.
CHAN_LEVELS_LIST_HEADER
	Configuración de nivel de acceso para canal 12%s:
CHAN_LEVELS_LIST_DISABLED
	    %-*s  (deshabilitado)
CHAN_LEVELS_LIST_FOUNDER
	    %-*s  (fundador solamente)
CHAN_LEVELS_LIST_NORMAL
	    %-*s  %d
CHAN_LEVELS_RESET
	Niveles de acceso de 12%s configurados a niveles originales.

# INFO responses
CHAN_INFO_SYNTAX
	12INFO <canal> [ALL]
CHAN_INFO_HEADER
	Información de canal 12%s:
CHAN_INFO_ACTIVED
	Estado: ACTIVO
CHAN_INFO_SUSPENDED
	Estado: SUSPENDIDO
CHAN_INFO_FOUNDER
	Fundador: %s (%s)
CHAN_INFO_NO_FOUNDER
	Fundador: %s
CHAN_INFO_SUCCESSOR
	Sucesor: %s (%s)
CHAN_INFO_NO_SUCCESSOR
	Sucesor: %s
CHAN_INFO_DESCRIPTION
	Descripción: %s
CHAN_INFO_ENTRYMSG
	Mensaje de entrada: %s
CHAN_INFO_TIME_REGGED
	Registrado: %s
CHAN_INFO_LAST_USED
	Ultimo uso: %s
CHAN_INFO_LAST_TOPIC
	Ultimo tópico: %s
CHAN_INFO_TOPIC_SET_BY
	Tópico ajustado por: %s
CHAN_INFO_URL
	URL: 12%s
CHAN_INFO_EMAIL
	E-mail: 12%s
CHAN_INFO_OPTIONS
	Opciones: %s
CHAN_INFO_OPT_PRIVATE
	Privado
CHAN_INFO_OPT_KEEPTOPIC
	Retención de tópico
CHAN_INFO_OPT_TOPICLOCK
	Candado de tópico
CHAN_INFO_OPT_SECUREOPS
	Secure Ops
CHAN_INFO_OPT_LEAVEOPS
	Leave Ops
CHAN_INFO_OPT_RESTRICTED
	Acceso restringido
CHAN_INFO_OPT_SECURE
	Seguro
CHAN_INFO_OPT_NONE
	Ninguna
CHAN_INFO_MODE_LOCK
	Candado de modo: %s
CHAN_INFO_NO_EXPIRE
	Expiración del canal:12 no expirará.

# LIST responses
CHAN_LIST_SYNTAX
	12LIST <patron>
CHAN_LIST_HEADER
	Lista de entradas que satisfacen 12%s:
CHAN_LIST_FORMAT
	    %-20s  %s
CHAN_LIST_END
	Fin de lista - 12%d/12%d entradas mostradas.

# INVITE responses
CHAN_INVITE_SYNTAX
	12INVITE <canal>

# OP responses
CHAN_OP_SYNTAX
	12OP <canal> <nick>

# DEOP responses
CHAN_DEOP_SYNTAX
	12DEOP <canal> <nick>

# VOICE responses
CHAN_VOICE_SYNTAX
	12VOICE <canal> <nick>

# DEVOICE responses
CHAN_DEVOICE_SYNTAX
	12DEVOICE <canal> <nick>

# KICK responses
CHAN_KICK_SYNTAX
	12KICK <canal> <nick> [razón]

# BAN responses
CHAN_BAN_SYNTAX
	12BAN <canal> <mascara>

#UNBAN responses
CHAN_UNBAN_SYNTAX
	12UNBAN <canal>
CHAN_UNBANNED
	Has sido desbaneado de 12%s.

# CLEAR responses
CHAN_CLEAR_SYNTAX
	12CLEAR <canal> <opcion>
CHAN_CLEARED_BANS
	Todos los bans en canal 12%s han sido borrados.
CHAN_CLEARED_MODES
	Todos los modos en canal 12%s han sido borrados.
CHAN_CLEARED_OPS
	Modo +o de operadores han sido desactivados del canal 12%s.
CHAN_CLEARED_VOICES
	Modo +v de algunos usuarios han sido desactivados del canal 12%s.
CHAN_CLEARED_USERS
	Todos los usuarios del han sido kickeados del canal 12%s.

# RESET responses
CHAN_RESET_SYNTAX
	12RESET <canal>


# GETPASS responses
CHAN_GETPASS_SYNTAX
	12GETPASS <canal>
CHAN_GETPASS_UNAVAILABLE
	Comando 12GETPASS no esta disponible cuando encripción esta en uso.
CHAN_GETPASS_PASSWORD_IS
	La contraseña del canal 12%s es 12%s.

# SUSPEND responses
CHAN_SUSPEND_SYNTAX
	12SUSPEND <canal> <motivo>
CHAN_SUSPEND_SUCCEEDED
	El canal 12%s esta 12SUSPENDido.
CHAN_SUSPEND_FAILED
	No fue posible suspender el canal 12%s!
	

# FORBID responses



CHAN_FORBID_SYNTAX
	12FORBID <canal> <motivo>
CHAN_FORBID_SUCCEEDED
	Canal 12%s esta prohibido de ser utilizado.
CHAN_FORBID_FAILED
	No fue posible prohibir el canal 12%s!

###########################################################################
#
# Mensajes de MemoServ
#
###########################################################################

# Automatic messages
MEMO_HAVE_NEW_MEMO
	3Eo! tienes 1 mensaje nuevo.
MEMO_HAVE_NEW_MEMOS
	3Eo! tienes %d mensajes nuevos.
MEMO_TYPE_READ_LAST
	Escribe 12/msg %s READ LAST para leer el último.
MEMO_TYPE_READ_NUM
	Escribe 12/msg %s READ %d para leerlo.
MEMO_TYPE_LIST_NEW
	Escribe 12/msg %s LIST NEW para listar los mensajes nuevos.
MEMO_AT_LIMIT
	4Atención: has llegado al máximo numero de mensajes (12%d). No podrás recibir mensajes nuevos hasta que borres algunos de los mensajes que tienes.
MEMO_OVER_LIMIT
	4Atención: has pasado el limite máximo de mensajes (12%d). No podrás recibir mensajes nuevos hasta que borres algunos de los mensajes que tienes.
MEMO_NEW_MEMO_ARRIVED
	Tienes un mensaje nuevo de 12%s. Escribe 12/msg %s READ %d para leerlo.
# Multi-use responses
MEMO_HAVE_NO_MEMOS
	No tienes mensajes.
MEMO_X_HAS_NO_MEMOS
	12%s no tiene mensajes.
MEMO_DOES_NOT_EXIST
	Mensaje 12%d no existe!
MEMO_LIST_NOT_FOUND
	No han encontrado memos.

# SEND responses
MEMO_SEND_SYNTAX
	12SEND {nick | canal} <texto>
MEMO_SEND_DISABLED
	Lo siento, el envío de mensajes esta temporalmente deshabilitado.
MEMO_SEND_PLEASE_WAIT
	Por favor espere 12%d segundos antes de utilizar el comando 12SEND nuevamente.
MEMO_X_GETS_NO_MEMOS
	12%s no puede recibir mensajes.
MEMO_X_HAS_TOO_MANY_MEMOS
	12%s tiene demasiados mensajes y no puede recibir mas.
MEMO_SENT
	Mensaje enviado a 12%s.

# LIST responses
MEMO_LIST_SYNTAX
	12LIST [canal] [list | NEW]
MEMO_HAVE_NO_NEW_MEMOS
	No tienes nuevos mensajes.
MEMO_X_HAS_NO_NEW_MEMOS
	12%s no tiene nuevos mensajes.
MEMO_LIST_MEMOS
	Mensajes para 12%s. Para leerlo, escribe: 12/msg %s READ <numero>
MEMO_LIST_NEW_MEMOS
	Nuevos mensajes para 12%s. Para leerlos, escribe: 12/msg %s READ <numero>
MEMO_LIST_CHAN_MEMOS
	Mensaje para 12%s. Para leerlos, escribe 12/msg %s READ %s <numero>
MEMO_LIST_CHAN_NEW_MEMOS
	Nuevos mensajes para 12%s. Para leerlos, escribe: 12/msg %s READ %s <numero>
MEMO_LIST_HEADER
	 Num  Remitente         Día/Hora
MEMO_LIST_FORMAT
	%c%3d  %-16s  %s

# READ responses
MEMO_READ_SYNTAX
	12READ [canal] {list | LAST | NEW}
MEMO_HEADER
	Mensaje 12%d de 12%s (12%s). Para borrarlo, escriba: 12/msg %s DEL %d
MEMO_CHAN_HEADER
	Mensaje 12%d de 12%s (12%s). Para borrarlo, escriba: 12/msg %s DEL %s %d
MEMO_TEXT
	%s

# DEL responses
MEMO_DEL_SYNTAX
	12DEL [canal] {numero | list | ALL}
MEMO_DELETED_NONE
	Ningún mensaje ha sido borrado.
MEMO_DELETED_ONE
	Mensaje 12%d ha sido borrado.
MEMO_DELETED_SEVERAL
	12%s mensajes han sido borrados.
MEMO_DELETED_ALL
	Todos sus mensajes han sido borrados.

# SET responses
MEMO_SET_SYNTAX
	12SET <opcion> <parametros>
MEMO_SET_DISABLED
	Lo siento, la opción de ajuste de mensajes esta temporalmente deshabilitada.
MEMO_SET_UNKNOWN_OPTION
	4Opción SET 12%s desconocida.

# SET NOTIFY responses
MEMO_SET_NOTIFY_SYNTAX
	12SET NOTIFY {ON | LOGON | NEW | OFF}
MEMO_SET_NOTIFY_ON
	12%s te notificara tus mensajes cuando te conectes o cuando te sean enviados.
MEMO_SET_NOTIFY_LOGON
	12%s te notificara tus mensajes cuando te conectes o canceles tu modo /AWAY.
MEMO_SET_NOTIFY_NEW
	12%s te notificara tus mensajes cuando te sean enviados.
MEMO_SET_NOTIFY_OFF
	12%s no te notificara tus mensajes.

# SET LIMIT responses
MEMO_SET_LIMIT_SYNTAX
	12SET LIMIT [canal_] <limite>
MEMO_SET_LIMIT_SERVADMIN_SYNTAX
	12SET LIMIT [ nick | canal] {limite | NONE} [HARD]
MEMO_SET_YOUR_LIMIT_FORBIDDEN
	No puedes cambiar tu limite de mensajes.
MEMO_SET_LIMIT_FORBIDDEN
	El limite de mensajes para 12%s no puede ser cambiado.
MEMO_SET_YOUR_LIMIT_TOO_HIGH
	No puedes ajustar tu limite de mensajes mayor que 12%d.
MEMO_SET_LIMIT_TOO_HIGH
	No puedes ajustar el limite de mensajes para 12%s mas alto que 12%d.
MEMO_SET_LIMIT_OVERFLOW
	Limite de mensajes muy alto; limitando a 12%d.
MEMO_SET_YOUR_LIMIT
	Tu limite de mensaje ha sido ajustado a 12%d.
MEMO_SET_YOUR_LIMIT_ZERO
	No podrás recibir mensajes.
MEMO_SET_LIMIT
	Limite de mensajes para 12%s ajustado a 12%d.
MEMO_SET_LIMIT_ZERO
	Limite de mensajes para 12%s ajustado a 120.
MEMO_UNSET_YOUR_LIMIT
	Tu limite de mensajes ha sido deshabilitado.
MEMO_UNSET_LIMIT
	Limite de mensajes desactivado para 12%s.

# INFO responses
MEMO_INFO_SYNTAX
	12INFO [canal]
MEMO_INFO_SERVADMIN_SYNTAX
	12INFO [nick | canal]
MEMO_INFO_NO_MEMOS
	No tienes mensajes.
MEMO_INFO_MEMO
	Tienes12 1 mensaje.
MEMO_INFO_MEMO_UNREAD
	Tienes12 1 mensaje, y no ha sido leído.
MEMO_INFO_MEMOS
	Tienes 12%d mensajes.
MEMO_INFO_MEMOS_ONE_UNREAD
	Tienes %d mensajes, de los cuales 1 no ha sido leído.
MEMO_INFO_MEMOS_SOME_UNREAD
	Tienes 12%d mensajes, de los cuales 12%d no han sido leídos.
MEMO_INFO_MEMOS_ALL_UNREAD
	Tienes 12%d mensajes; de los cuales ninguno ha sido leído.
MEMO_INFO_LIMIT
	Tu limite de mensajes es 12%d.
MEMO_INFO_HARD_LIMIT
	Tu limite de mensajes es 12%d, y no puede ser cambiado.
MEMO_INFO_LIMIT_ZERO
	Tu limite de mensajes es12 0; no recibirás ningún nuevo mensaje.
MEMO_INFO_HARD_LIMIT_ZERO
	Tu limite de mensajes es12 0; no recibirás ningún nuevo mensaje. No puedes cambiar este limite.
MEMO_INFO_NO_LIMIT
	No tienes limite en la cantidad de mensajes que puede guardar.
MEMO_INFO_X_NO_MEMOS
	12%s no tiene mensajes.
MEMO_INFO_X_MEMO
	12%s tiene actualmente12 1 mensaje.
MEMO_INFO_X_MEMO_UNREAD
	12%s tiene actualmente1 1 mensaje, y no ha sido leído aun.
MEMO_INFO_X_MEMOS
	12%s tiene actualmente 12%d mensajes.
MEMO_INFO_X_MEMOS_ONE_UNREAD
	12%s tiene actualmente 12%d mensajes, de los cuales12 1 esta sin leer.
MEMO_INFO_X_MEMOS_SOME_UNREAD
	12%s tiene actualmente 12%d mensajes, de los cuales 12%d están sin leer.
MEMO_INFO_X_MEMOS_ALL_UNREAD
	12%s tiene actualmente 12%d mensajes; de los cuales todos están sin leer.
MEMO_INFO_X_LIMIT
	El limite de mensajes de 12%s es 12%d.
MEMO_INFO_X_HARD_LIMIT
	El limite de mensajes de 12%s es 12%d, y no puede ser cambiado por el usuario.
MEMO_INFO_X_NO_LIMIT
	12%s no tiene limite de mensajes.

###########################################################################
#
# Mensajes de OperServ
#
###########################################################################

# General messages
OPER_BOUNCY_MODES
	Services no puede cambiar modos.
	Tus servidores están configurados correctamente?
OPER_BOUNCY_MODES_U_LINE
	Services no puede cambiar modos.
	La líneas U: de sus servidores están configurados correctamente?

# GLOBAL responses
OPER_GLOBAL_SYNTAX
	12GLOBAL <message>

# STATS responses
OPER_STATS_UNKNOWN_OPTION
	4Opción STATS 12%s desconocida.
OPER_STATS_CURRENT_USERS
	Usuarios actuales : 12%d (12%d ircops)
OPER_STATS_MAX_USERS
	Máximo de usuarios: 12%d (12%s)
# This is ugly, but at least it's language-portable...
OPER_STATS_UPTIME_DHMS
	Services activos 12%d días, 12%02d:%02d
OPER_STATS_UPTIME_1DHMS
	Services activos 12%d día, 12%02d:%02d
OPER_STATS_UPTIME_HMS
	Services activos 12%d horas, 12%d minutos
OPER_STATS_UPTIME_HM1S
	Services activos 12%d horas, 12%d minutos
OPER_STATS_UPTIME_H1MS
	Services activos 12%d horas, 12%d minuto
OPER_STATS_UPTIME_H1M1S
	Services activos 12%d horas, 12%d minuto
OPER_STATS_UPTIME_1HMS
	Services activos 12%d hora, 12%d minutos
OPER_STATS_UPTIME_1HM1S
	Services activos 12%d hora, 12%d minutos
OPER_STATS_UPTIME_1H1MS
	Services activos 12%d hora, 12%d minuto
OPER_STATS_UPTIME_1H1M1S
	Services activos 12%d hora, 12%d minuto
OPER_STATS_UPTIME_MS
	Services activos 12%d minutos, 12%d segundos
OPER_STATS_UPTIME_M1S
	Services activos 12%d minutos, 12%d segundos
OPER_STATS_UPTIME_1MS
	Services activos 12%d minuto, 12%d segundos
OPER_STATS_UPTIME_1M1S
	Services activos 12%d minuto, 12%d segundo
OPER_STATS_BYTES_READ
	Bytes leidos   : 12%5d kB
OPER_STATS_BYTES_WRITTEN
	Bytes escritos : 12%5d kB
OPER_STATS_SERVER_MEM
	Servers : 12%6d registros, 12%5d kB
OPER_STATS_USER_MEM
	Usuarios: 12%6d registros, 12%5d kB
OPER_STATS_CHANNEL_MEM
	Canales : 12%6d registros, 12%5d kB
OPER_STATS_NICKSERV_MEM
	NiCK    : 12%6d registros, 12%5d kB
OPER_STATS_CHANSERV_MEM
	CHaN    : 12%6d registros, 12%5d kB
OPER_STATS_OPERSERV_MEM
	OPeR    : 12%6d registros, 12%5d kB
OPER_STATS_AKILL_COUNT
	Numero actual de GLINEs: 12%d
OPER_STATS_AKILL_EXPIRE_DAYS
	Tiempo default para expirar GLINE: 12%d días
OPER_STATS_AKILL_EXPIRE_DAY
	Tiempo default para expirar GLINE:12 1 día
OPER_STATS_AKILL_EXPIRE_HOURS
	Tiempo default para expirar GLINE: 12%d horas
OPER_STATS_AKILL_EXPIRE_HOUR
	Tiempo default para expirar GLINE:12 1 hora
OPER_STATS_AKILL_EXPIRE_MINS
	Tiempo default para expirar GLINE: 12%d minutos
OPER_STATS_AKILL_EXPIRE_MIN
	Tiempo default para expirar GLINE:12 1 minuto
OPER_STATS_AKILL_EXPIRE_NONE
	Tiempo default para expirar GLINE: 12No expira
OPER_STATS_SESSIONS_MEM
	Sesiones: 12%6d registros, 12%5d kB

# MODE responses
OPER_MODE_SYNTAX
	12MODE <canal> <modos>

# CLEARMODES responses
OPER_CLEARMODES_SYNTAX
	12CLEARMODES <canal> [ALL]

OPER_CLEARMODES_DONE
	Modos y bans del canal 12%s han sido limpiados.

OPER_CLEARMODES_ALL_DONE
	Todos los modos del canal 12%s han sido limpiados.

# KICK responses
OPER_KICK_SYNTAX
	12KICK <canal> <nick> <motivo>

# ADMIN responses
OPER_ADMIN_SYNTAX
	12{CREGADMIN|COADMIN|ADMIN} {ADD | DEL | LIST} [nick]
OPER_ADMIN_ADD_SYNTAX
	12{CREGADMIN|COADMIN|ADMIN} ADD <nick>
OPER_ADMIN_DEL_SYNTAX
	12{CREGADMIN|COADMIN|ADMIN} DEL <nick>

OPER_ADMIN_SKELETON
	Services esta en modo skeleton; el comando 12%s no esta disponible.
OPER_ADMIN_EXISTS
	12%s ya está en la lista de %s de Services.
OPER_ADMIN_ADDED
	12%s agregado a la lista de %s de Services.
OPER_ADMIN_TOO_MANY
	Demasiadas entradas (12%d) en la lista de %s de Services; no se puede agregar mas.
OPER_ADMIN_REMOVED
	12%s borrado de la lista de %s de Services.
OPER_ADMIN_NOT_FOUND
	12%s no fue encontrado en la lista de %s de Services.
OPER_ADMIN_LIST_HEADER
	Lista de %s de Services:

# OPER responses
OPER_OPER_SYNTAX
	12{PREOPER|OPER} {ADD | DEL | LIST} [nick]
OPER_OPER_ADD_SYNTAX
	12{PREOPER|OPER} ADD <nick>
OPER_OPER_DEL_SYNTAX
	12{PREOPER|OPER} DEL <nick>
OPER_OPER_SKELETON
	Services esta en modo skeleton; el comando %s no esta disponible.
OPER_OPER_EXISTS
	12%s ya estaba en la lista de %s de Services.
OPER_OPER_ADDED
	12%s agregado a la lista de %s de Services.
OPER_OPER_TOO_MANY
	Muchas entradas (12%d) en la lista de %s de Services; no se pueden agregar mas.
OPER_OPER_REMOVED
	12%s borrado de la lista de %s de Services.
OPER_OPER_NOT_FOUND
	12%s no fue encontrado en la lista de %s de Services.
OPER_OPER_LIST_HEADER
	Lista de %s de Services:

# AKILL responses
OPER_AKILL_SYNTAX
	12GLINE {ADD | DEL | LIST | VIEW} [[+expiracion] <mascara> [motivo]]
OPER_AKILL_ADD_SYNTAX
	12GLINE ADD [+expiracion] <mascara> <motivo>
OPER_AKILL_DEL_SYNTAX
	12GLINE DEL <mascara>
OPER_TOO_MANY_AKILLS
	La lista de GLINEs esta 12llena.
OPER_AKILL_NO_NICK
	4Atención: Las máscaras de 12GLINE no pueden contener nicks;
	4NO incluyas la parte de nick en su mascara.
OPER_AKILL_ADDED
	12%s agregado a la lista de GLINEs.
OPER_AKILL_REMOVED
	12%s eliminado de la lista de GLINEs.
OPER_AKILL_NOT_FOUND
	12%s no fue encontrado en la lista de GLINEs.
OPER_AKILL_LIST_HEADER
	Lista actual de 12GLINEs:
# mask, reason
OPER_AKILL_LIST_FORMAT
	%-32s  %s
# mask, set-by, set-time, expires, reason
OPER_AKILL_VIEW_FORMAT
	%s (por %s en %s; %s)
	    %s
OPER_AKILL_NO_EXPIRE
	no expira
OPER_AKILL_EXPIRES_SOON
	expira en la siguiente actualización de base de datos
OPER_AKILL_EXPIRES_M
	expira en 12%d minutos
OPER_AKILL_EXPIRES_1M
	expira en 12%d minuto
OPER_AKILL_EXPIRES_HM
	expira en 12%d horas, 12%d minutos
OPER_AKILL_EXPIRES_H1M
	expira en 12%d horas, 12%d minuto
OPER_AKILL_EXPIRES_1HM
	expira en 12%d hora, 12%d minutos
OPER_AKILL_EXPIRES_1H1M
	expira en 12%d hora, 12%d minuto
OPER_AKILL_EXPIRES_D
	expira en 12%d días
OPER_AKILL_EXPIRES_1D
	expira en 12%d día

# SET responses
OPER_SET_SYNTAX
	12SET <opcion> <parametros>
OPER_SET_IGNORE_ON
	Ignore code sera usado.
OPER_SET_IGNORE_OFF
	Ignore code no sera usado.
OPER_SET_IGNORE_ERROR
	Parámetro para IGNORE debe ser ON o OFF.
OPER_SET_READONLY_ON
	Services esta ahora en modo leer-solamente read-only.
OPER_SET_READONLY_OFF
	Services esta ahora en modo leer-escribir read-write.
OPER_SET_READONLY_ERROR
	Parámetro para READONLY debe ser ON o OFF.
OPER_SET_DEBUG_ON
	Services esta ahora en modo debug.
OPER_SET_DEBUG_OFF
	Services esta ahora en modo non-debug.
OPER_SET_DEBUG_LEVEL
	Services esta ahora en modo debug (nivel %d).
OPER_SET_DEBUG_ERROR
	Parámetro para DEBUG debe ser ON, OFF, o un numero positivo.
OPER_SET_UNKNOWN_OPTION
	Opción %s desconocida.

# JUPE responses
OPER_JUPE_SYNTAX
	12JUPE <servidor> [motivo]

# RAW responses
OPER_RAW_SYNTAX
	12RAW <text>

# UPDATE responses
OPER_UPDATING
	4Actualizando bases de datos.

# RESTART responses
OPER_CANNOT_RESTART
	SERVICES_BIN no definida; no es posible reiniciar. Ejecute nuevamente el script 12./configure y recompile Services para habilitar el comando 12RESTART.

# LISTIGNORE responses
OPER_IGNORE_LIST
	Lista de ignoración de Services:
OPER_IGNORE_LIST_EMPTY
	Lista de ignoración esta vacía.

# ROTATELOG responses
OPER_ROTATELOG_FILE_EXISTS
	No ha podido renombrar el log, un archivo con el nombre 12%s todavia existe!
OPER_ROTATELOG_RENAME_DONE
	Log renombrado a 12%s
OPER_ROTATELOG_RENAME_FAIL
	Log no ha podido renombrarlo: 12%s
OPER_ROTATELOG_OPEN_LOG_FAIL
	Logueando no ha podido resetear: 12%s

###########################################################################
#
# Mensajes de CyberServ
#
###########################################################################

CYBER_NO_ADMIN_CYBER
	No eres un admin de un Cyber.
CYBER_NICK_NOT_IN_CYBER
	12%s no está en tu Cyber.

#Respuestas a KILL
CYBER_KILL_SYNTAX
	12KILL <nick> [motivo]
CYBER_KILL_SUCCEEDED
	El usuario 12%s de tu cyber KILLeado!.

#Respuestas a Gline
CYBER_GLINE_SYNTAX
	12GLINE <nick> [motivo]
CYBER_GLINE_SUCCEEDED
	El usuario 12%s de tu cyber GLINEeado!.

#Respuestas a UNBAN
CYBER_UNBAN_SYNTAX
	12UNBAN <canal>
CYBER_UNBAN_FAILED
	No hay ban en 12%s que afecte a tu cyber.
CYBER_UNBAN_SUCCEEDED
	El Cyber desbaneado de 12%s.

# EXCEPTION responses
OPER_EXCEPTION_SYNTAX
	12ILINE {ADD | DEL | MOVE | LIST | VIEW} [parametros]
OPER_EXCEPTION_ADD_SYNTAX
	12ILINE ADD [+expiracion] <mascara> <limite> <razon>
OPER_EXCEPTION_DEL_SYNTAX
	12ILINE DEL {mascara | list}
OPER_EXCEPTION_MOVE_SYNTAX
	12ILINE MOVE <numero> <posicion>
OPER_EXCEPTION_DISABLED
	El control de clones está 12desactivado.
OPER_EXCEPTION_ALREADY_PRESENT
	El host 12%s ya tiene concedidos clones.
OPER_EXCEPTION_TOO_MANY
	La lista de clones está 12llena.
OPER_EXCEPTION_ADDED
	Añadida Iline para 12%s, 12%d clones.
OPER_EXCEPTION_MOVED
	Iline para 12%s (12#%d) movida a la posicion 12%d.
OPER_EXCEPTION_NO_SUCH_ENTRY
	No existe entrada (12#%d) en la lista de Ilines.
OPER_EXCEPTION_NOT_FOUND
	El host 12%s no tiene concedidos clones.
OPER_EXCEPTION_NO_MATCH
	No hay ningun 12HOST en la lista de Ilines.
OPER_EXCEPTION_DELETED
	Iline para 12%s borrada en la lista de Ilines.
OPER_EXCEPTION_DELETED_ONE
	Borrada12 1 entrada en la lista de Ilines.
OPER_EXCEPTION_DELETED_SEVERAL
	Borradas 12%d entradas en la lista de ilines.
OPER_EXCEPTION_LIST_HEADER
	Lista de 12ILINES:
# mask, limit
OPER_EXCEPTION_LIST_FORMAT
	%3d  %4d   %s
OPER_EXCEPTION_LIST_COLHEAD
	Num  Limite  Host
# mask, set-by, set-time, expires, limit, reason
OPER_EXCEPTION_VIEW_FORMAT
	%3d.  %s  (por %s en %s; %s)
	    Limite: %-4d  - %s
OPER_EXCEPTION_INVALID_LIMIT
	Número de clones no valido. It must be a valid integer greater than or equal
OPER_EXCEPTION_INVALID_HOSTMASK
	Host no valido. Only real hostmasks are valid as exceptions are not m

# SESSION responses
OPER_SESSION_SYNTAX
	12CLONES {LIST numero | VIEW host}
OPER_SESSION_LIST_SYNTAX
	12CLONES LIST <numero>
OPER_SESSION_VIEW_SYNTAX
	12CLONES VIEW <host>
OPER_SESSION_DISABLED
	El control de clones esta 12desactivado.
OPER_SESSION_INVALID_THRESHOLD
	Valor incorrecto. El valor debe ser mayor que 1.
OPER_SESSION_NOT_FOUND
	El host 12%s no tiene clones.
OPER_SESSION_LIST_HEADER
	Hosts con por lo menos 12%d clones:
OPER_SESSION_LIST_COLHEAD
	Clones  Host
# limit, host
OPER_SESSION_LIST_FORMAT
	%6d    %s
# host, sessions, limit
OPER_SESSION_VIEW_FORMAT
	El host 12%s tiene actualmente 12%d clones con un limite de 12%d.
CYBER_CLONES_IS_CYBER
	Eres Cyber, tu host 12%s tiene ahora mismo 12%d clones (Limite 12%d clones)
CYBER_CLONES_NOT_IS_CYBER
	No eres Cyber

# KILLCLONES responses
OPER_KILLCLONES_SYNTAX
	12KILLCLONES <nick>
OPER_KILLCLONES_UNKNOWN_NICK
	No ha sido encontrado el nick 12%s.
CYBER_KILLCLONES_SUCCEEDED
	Han sido killeados 12%d clones del nick 12%s.

###########################################################################
#
# News system messages
#
###########################################################################

NEWS_LOGON_TEXT
	[4Noticias de la red - 12%s] %s
NEWS_OPER_TEXT
	[4Noticias OPERS/IRCOPS - 12%s] %s

NEWS_LOGON_SYNTAX
	Sintaxis: 12LOGONNEWS {ADD|DEL|LIST} [texto|numero]
NEWS_LOGON_LIST_HEADER
	Logon news items:
NEWS_LOGON_LIST_ENTRY
	%5d (%s by %s)
	    %s
NEWS_LOGON_LIST_NONE
	There is no logon news.
NEWS_LOGON_ADD_SYNTAX
	Sintaxis: 12LOGONNEWS ADD <text>
NEWS_LOGON_ADD_FULL
	News list is full!
NEWS_LOGON_ADDED
	Added new logon news item (12#%d).
NEWS_LOGON_DEL_SYNTAX
	Sintaxis: LOGONNEWS DEL {numero | ALL}
NEWS_LOGON_DEL_NOT_FOUND
	Logon news item 12#%d not found!
NEWS_LOGON_DELETED
	Logon news item 12#%d deleted.
NEWS_LOGON_DEL_NONE
	No logon news items to delete!
NEWS_LOGON_DELETED_ALL
	All logon news items deleted.

NEWS_OPER_SYNTAX
	Sintaxis: 12OPERNEWS {ADD|DEL|LIST} [texto|numero]
NEWS_OPER_LIST_HEADER
	Oper news items:
NEWS_OPER_LIST_ENTRY
	%5d (%s by %s)
	    %s
NEWS_OPER_LIST_NONE
	There is no oper news.
NEWS_OPER_ADD_SYNTAX
	Syntax: 12OPERNEWS ADD <texto>
NEWS_OPER_ADD_FULL
	News list is full!
NEWS_OPER_ADDED
	Added new oper news item (12#%d).
NEWS_OPER_DEL_SYNTAX
	Sintaxis: OPERNEWS DEL {numero | ALL}
NEWS_OPER_DEL_NOT_FOUND
	Oper news item 12#%d not found!
NEWS_OPER_DELETED
	Oper news item 12#%d deleted.
NEWS_OPER_DEL_NONE
	No oper news items to delete!
NEWS_OPER_DELETED_ALL
	All oper news items deleted.

NEWS_HELP_LOGON
	Sintaxis: 12LOGONNEWS ADD <texto>
	          12LOGONNEWS DEL {numero | ALL}
	          12LOGONNEWS LIST
	
	Editar o mostrar la lista de mensajes de bienvenida de  noticias.
	Cuando un usuario conecta a la red, esos mensjes deben ser
	enviados a ellos. Si hay mas de 3 mensajes, solo se enviará los
	3 mensajes mas recientes.
	
	12LOGONNEWS LIST debe ser usado por algun IRCOP para listar los
	mensajes de noticias actuales. 12ADD y 12DEL solo podrán usarlos
	por 4Administradores de Services.
	
NEWS_HELP_OPER
	Sintaxis: 12OPERNEWS ADD texto
	          12OPERNEWS DEL {numero | ALL}
	          12OPERNEWS LIST
	
	Editar o mostrar la lista de mensajes de IRCOPS de noticias.
	Cuando un usuario hace IRCOP (con el comando /OPER), esos
	mensajes deben ser enviados a ellos.Si hay mas de 3 mensajes,
	solo se enviará los 3 mensajes mas recientes.
	
	12OPERNNEWS LIST debe ser usado por algun IRCOP para listar los
	mensajes de noticias actuales. 12ADD y 12DEL solo podrán usarlos
	por 4Administradores de Services

###########################################################################
#
# Mensajes de ayuda de NickServ
#
##########################################################################

NICK_HELP
	4%S te permite "registrar" un nick y previene que otros
	hagan uso de el. Si el nick NO se utiliza durante
	12%d días, caducará el registro. Los siguientes comandos
	permiten el registro y mantenimiento de nicks; para 
	utilizarlos, escribe 12/msg %S comando.

	Para más información sobre un comando especifico, escribe
	12/msg %S HELP comando
	
	    12REGISTER   Registra un nick
#	    12IDENTIFY   Identificarse con tu contraseña
	    12INFO       Muestra la informacion de un nick
	    12STATUS     Muestra el estado de un nick
	    12LIST       Lista los nicks registrados
	    12SET        Ajusta las opciones para tu nick
#
#	    12GHOST      Desconectar el nick con una conexion fantasma
#	    12RECOVER    Desconecta a otro usuario que está utilizando tu nick
#	    12RELEASE    Toma custodia de tu nick después de utilizar el
#	                comando RECOVER
	 
	4NOTA: Este servicio esta diseñado para proveer un modo en
	que los usuarios de IRC aseguran que su identidad no sea
	suplantada. Su intención 4NO es para facilitar el "robo"
	de nicks o otras acciones mal intencionadas. Abuso de
	4%S resultara, por lo menos, de la perdida de el/los
	nick(s) abusivo(s).

NICK_HELP_EXPIRE_ZERO
	4%S te permite "registrar" un nick y previene que otros
	hagan uso de el. Los siguientes comandos permiten el registro
	y mantenimiento de nicks; para utilizarlos,
	escribe 12/msg %S comando.
	
	Para más información sobre un comando especifico, escribe
	12/msg %S HELP comando
	
	    12REGISTER   Registra un nick
	    12IDENTIFY   Identificarse con tu contraseña
	    12ACCESS     Modificar la lista de direcciones autorizadas
	    12LINK       Hacer tu nick sea un alias de otro
	    12UNLINK	   Deshace un link de tu nick
	    12INFO       Muestra la informacion de un nick
	    12STATUS     Muestra el estado de un nick
	    12LIST       Lista los nicks registrados
	    12SET        Para ajustar opciones, incluyendo protección de kill
	    12DROP       Cancela el registro de un nick
	    12GHOST      Desconectar el nick con una conexion fantasma
	    12RECOVER    Desconecta a otro usuario que eestá utilizando tu nick
	    12RELEASE    Toma custodia de tu nick después de utilizar el
	                comando RECOVER
	 
	4NOTA: Este servicio esta diseñado para proveer un modo en
	que los usuarios de IRC aseguran que su identidad no sea
	suplantada. Su intención 4NO es para facilitar el "robo"
	de nicks o otras acciones mal intencionadas. Abuso de
	4%S resultara, por lo menos, de la perdida de el/los
	nick(s) abusivo(s).

NICK_HELP_REGISTER
	Sintaxis: 12REGISTER <direccion@correo.electronico>
	
	Registra tu nick la base de datos de 12%S.
	 
	La dirección de Email ha de ser una dirección REAL,
	para que puedas recibir el mensaje con la clave
	temporal de acceso en dicho buzón. No aceptamos
	anonimizadores ni cuentas de correo web al estilo
	de hotmail.com o iname.com para evitar abusos en el
	proceso de registro de Nicks.
	 
	Una vez identificado siguiendo los pasos indicados
	en el mensaje que te enviaremos, podras utilizar los
	comandos 12SET y 12ACCESS para configurar los
	parámetros de tu nick como desees.

NICK_HELP_IDENTIFY
	Sintaxis: 12IDENTIFY <contraseña>
	
	Notifica a 12%S que usted es el verdadero dueño de este
	nick.  Muchos de los comandos requieren que te indentifiques
	con este comando antes de poder utilizarlos. La
	contraseña debe ser la misma que utilizo con el comando
	12REGISTER.

NICK_HELP_DROP
	Sintaxis: 12DROP
	
	Elimina tu nick de la base de datos de 12%S. Un nick
	que ha sido eliminado esta disponible para que cualquier
	persona lo vuelva a registrar.
	
	Para utilizar este comando , tienes que identificarte
	con tu contraseña (12/msg %S HELP IDENTIFY para mas
	información).

NICK_HELP_ACCESS
	Sintaxis: 12ACCESS ADD <mascara>
	          12ACCESS DEL <mascara>
	          12ACCESS LIST
	
	Modifica o muestra la lista de acceso para su nick. Esta
	es la lista de direcciones que 12%S reconocerá automáticamente
	como validas para utilizar este nick. Si desea utilizar este
	nick desde otra dirección, necesita enviar el comando
	IDENTIFY para que 12%S te reconozca.
	
	Ejemplos:
	
	    12ACCESS ADD achurch@*.dragonfire.net
	        Permite el acceso al usuario 12achurch de cualquier
	        ordenador en el dominio 12dragonfire.net.
	
	    12ACCESS DEL achurch@*.dragonfire.net
	        Hace lo contrario de el comando anterior.
	
	    12ACCESS LIST
	        Muestra la lista de acceso.
	
NICK_HELP_LINK
	Sintaxis: 12LINK <nick> <contraseña>
	
	Crea un pseudónimo de tu nick, o tambien un 'alias'
	Cuando utilizas este comando, la lista de acceso de el
	nick que tienes es eliminada y reemplazada por la del
	pseudónimo de tu nick; todos los mensajes para el nick
	actual son agregados a la lista de mensajes del pseudónimo.
	(esto puede causar que el nick se exceda en su limite de
	mensajes, en este caso necesitarás borrar algunos de los
	mensajes antes de poder recibir nuevos mensajes para
	cualquiera de ambos nicks).
	
	Unas vez establecido el alias, tu nick será convertido
	transparentemente al nick nuevo en todas las bases de
	datos de Services, excepto para los comandos 12%S UNLINK y
	12DROP. Por ejemplo, puedes utilizar cualquiera de los
	nicks para leer los mismos mensajes (y los mensajes que se
	envían a cualquier de los nicks irán a la misma lista). Si
	se identifica para un nick, automáticamente se identificara
	para el otro. Si cualquiera de ambos nicks esta en la lista
	de acceso de un canal, ambos nicks tendrán los mismos
	privilegios y accesos.
	
	Para utilizar este comando, debes identificarte para
	tu nick actual (usando el comando 12IDENTIFY), y debe
	proporcionar la contraseña para el  nick del alias.

NICK_HELP_UNLINK
	Sintaxis: 12UNLINK
	
	Elimina el alias de tu nick. Cuando el alias se
	elimina, los modos de un nick y la lista de acceso del
	nick previamente linkado son copiados a su nick actual y
	pueden ser modificados independientemente.  Mensajes no son
	copiados; todos los mensajes permanecen con el nick
	anterior.

NICK_HELP_SET
	Sintaxis: 12SET <opción> <parámetros>
	
	Ajusta varios parámetros del nick. Una opción puede ser:
	
	    12PASSWORD   Ajusta la contraseña de tu nick
	    12LANGUAGE   Ajusta el lenguaje que Services utilizara cuando
	               te envié mensajes
	    12URL        Asocia un URL con tu nick
	    12KILL       Activa/desactiva la protección de kill
	    12SECURE     Activa/desactiva el modo SECURE para tu nick
	    12PRIVATE    Evita que su nick aparezca en 12/msg %S LIST
	    12HIDE       Esconde ciertas informaciones sobre tu nick
	    12IPVIRTUAL  Permite cambiar parte de la IP virtual.
	
	Para utilizar este comando es necesario que te identifiques antes
	con su contraseña (12/msg %S HELP IDENTIFY para mas
	información).
	
	Escribe 12/msg %S HELP SET <opción> para mas información
	sobre una opción especifica.

NICK_HELP_SET_PASSWORD
	Sintaxis: 12SET PASSWORD <nueva-contraseña>
	
	Cambia la contraseña que utiliza para identificarse como el
	dueño del nick.

NICK_HELP_SET_LANGUAGE
	Sintaxis: 12SET LANGUAGE <numero>
	
	Cambia el lenguaje que Services utiliza cuando envía
	mensajes a usted (por ejemplo cuando responde a un comando
	que usted envió). 12numero debe ser escogido de la siguiente
	lista de lenguajes soportados:

NICK_HELP_SET_URL
	Sintaxis: 12SET URL <url>
	
	Asocia un URL dado con tu nick. Este URL sera mostrado
	cuando solicite información de su nick con el comando 12INFO.

NICK_HELP_SET_EMAIL
	Sintaxis: 12SET EMAIL <dirección>
	
	Asocia la dirección de E-mail dada con tu nick. Esta
	dirección sera mostrada cuando alguien solicite información
	sobre su nick con el comando 12INFO.

NICK_HELP_SET_KILL
	Sintaxis: 12SET KILL {ON | QUICK | IMMED | OFF}
	
	Activa/desactiva la protección de kill para su nick. Con
	protección de kill activada, si algún otro usuario intenta
	utilizar tu nick, se le dará un minuto para que cambie a
	otro, si no sera removido forzosamente de IRC por 12%S.
	
	Si seleccione 12QUICK, el usuario solo tendrá 20 segundos para
	cambiar de nick en lugar de lo usual (60 segundos). Si
	seleccione 12IMMED, el usuario sera desconetcado (kill)
	inmediatamente 4sin ser avisado primero o darle una
	oportunidad de cambiar de nick, favor de no utilizar esta
	opción a menos que sea verdaderamente necesario. Los
	Administradores de la red pueden haber deshabilitado esta opción.

NICK_HELP_SET_SECURE
	Sintaxis: 12SET SECURE {ON | OFF}
	
	Activa/desactiva las funciones de seguridad de 12%S para
	tu nick. Con 12SECURE activado, usted debe de enviar su
	contraseña antes de que se le reconozca como el dueño de el
	nick, sin importar si su dirección este o no en la lista de
	acceso. Sin embargo, si aparece en la lista de acceso, 12%S
	no lo desconectara automáticamente, independientemente de
	como este la opción de 12KILL.

NICK_HELP_SET_PRIVATE
	Sintaxis: 12SET PRIVATE {ON | OFF}

	Activa/desactiva la opción privacidad de 12%S sobre tu
	nick. Con 12PRIVATE activado, tu nick no aparecerá en las
	listas de nicks generadas con el comando 12%S LIST.
	(Sin embargo, cualquiera que conozca tu nick puede obtener
	información usando el comando 12INFO.)

NICK_HELP_SET_HIDE
	Sintaxis: 12SET HIDE {EMAIL | USERMASK | QUIT} {ON | OFF}

	Permite evites que ciertas informaciones sean
	mostradas cuando alguien usa el comando 12%S INFO en tu
	nick. Usted puede esconder su E-mail (12EMAIL), ultima
	mascara user@host (12USERMASK), o su ultimo mensaje de salida
	(12QUIT). El segundo parámetro especifica que información
	debe ser mostrada (12OFF) o escondida (12ON).

NICK_HELP_SET_VHOST
	Sintaxis: 12SET IPVIRTUAL {<mascara> | OFF}

	Permite añadir/eliminar una entrada en la tabla W del IRCd, lo que
	facilita una dirección virtual semi-personalizada, con una parte
	generada a partir de la IP del cliente y otra a elegir medianta
	el parámetro <máscara>. Usando OFF podemos volver al formato "estándar"


NICK_HELP_RECOVER
	Sintaxis: 12RECOVER <nick> [contraseña]

	Permite que recuperes tu nick si alguien mas lo ha
	tomado; esto es lo mismo que 12%S hace automáticamente si
	alguien intenta utilizar un nick protegido con kill.

	Cuando usted envía este comando, 12%S conectara en línea a un
	usuario falso con el mismo nick que el usuario que usted
	desea recuperar. Esto causara que los servidores de IRC
	desconecten al otro usuario. El usuario falso permanecerá
	en línea por un minuto para asegurar que el otro usuario no
	se reconecte inmediatamente; después de ese minuto, usted
	puede tomar su nick. Alternativamente, use el comando
	12RELEASE (12/msg %S HELP RELEASE) para obtener tu nick
	mas rápido.
	
	Para usar el comando 12RECOVER para un nick, tu dirección
	actual como se muestra en un /WHOIS debe de estar en la
	lista de acceso de el nick, o usted debe de proporcionar la
	contraseña correcta para el nick.

NICK_HELP_RELEASE
	Sintaxis: 12RELEASE <nick> [contraseña]
	
	Ordena a 12%S eliminar cualquier espera en su nick
	causada por un kill automático de protección o por el uso de
	el comando 12RECOVER. Por defecto, la espera dura un minuto;
	este comando lo libera mas rápidamente.
	
	Para usar el comando 12RELEASE para un nick, su dirección
	actual como se muestra en un /WHOIS debe de estar en la
	lista de acceso del nick, o usted debe de proporcionar la
	contraseña correcta para el nick.

NICK_HELP_GHOST
	Sintaxis: 12GHOST <nick> [contraseña]
	
	Elimina una sesión "fantasma" de IRC que este utilizando tu
	nick. Una sesión "fantasma" es aquella que no esta
	realmente conectada, pero que un servidor de IRC acredita
	como activa por cualquier razón.  Típicamente esto sucede si
	su ordenador se cuelga o su conexión a Internet se
	desconecta por un periodo corto mientras esta en IRC.
	
	Para usar el comando 12GHOST para un nick, tu dirección
	actual como se muestra en un /WHOIS debe de estar en la
	lista de acceso de el nick, o usted debe de proporcionar la
	contraseña correcta para el nick.

NICK_HELP_INFO
	Sintaxis: 12INFO <nick>
	
	Muestra información sobre un nick dado, como el quien es
	el dueño, ultima hora y dirección utilizada, y las
	diferentes opciones del nick.

NICK_HELP_LIST
	Sintaxis: 12LIST <patrón>
	
	Lista todos los nicks registrados que concuerdan con un
	patrón dado en formato 12nick!user@host. Nicks con la opción
	de PRIVATE activada no serán mostrados.
	
	Ejemplos:
	
	    12LIST *!joeuser@foo.com
	         Lista los nicks registrados por joeuser@foo.com.

	    12LIST *Bot*!*@*
	        Lista todos los nicks registrados que contenga Bot
	        en su nombre (no importa mayúsculas).
		
	    12LIST *!*@*.bar.org
	        Lista todos los nicks registrados por usuarios del dominio 12bar.org.

NICK_HELP_STATUS
	Sintaxis: 12STATUS <nick1> <nick2>...
	
	Informa si el usuario de un nick es reconocido como el dueño
	de el mismo.  La respuesta tiene el siguiente formato:
	
	    12nick 12código-de-status
	
	donde 12nick es el nick enviado con el comando,
	12código-de-status es uno de los siguientes:

	    0 - usuario no esta conectado 12o nick no esta registrado
	    1 - usuario no reconocido como dueño del nick
	    2 - usuario reconocido por la lista de acceso solamente
	    3 - usuario reconocido por identificación con contraseña
	    4 - usuario con el nick suspendido
	
	Hasta 16 nicks pueden ser enviados con cada comando; el
	resto serán ignorados. No se genera mensaje de error si no
	se envía nick.

NICK_SERVADMIN_HELP
	
	Los siguientes comandos están disponibles para los
	4Operadores y Administradores de Services:
	    
	    12DROP       Cancela el registro de un nick
	    12SENDPASS   Manda la contraseña al mail
	    12GETPASS    Muestra la contraseña para un nick
	    12SUSPEND    Suspende el nick
	    12UNSUSPEND  Reactiva un nick suspendido
	    12FORBID     Evita que un nick sea utilizado
	    12UNFORBID   Libera un nick prohibido
	    12RENAME     Cambia un nick por otro generado aleatoriamente.
	    12USERIP     Obtiene la dirección IP/Host del usuario.
	
	4Administradores de Services pueden cancelar cualquier nick
	sin necesidad de identificarse por el mismo, y pueden ver la
	lista de acceso de cualquier nick (12/msg %S ACCESS LIST <nick>).

NICK_SERVADMIN_HELP_USERIP
	Sintaxis: 12USERIP <nick>
	
	Muestra la dirección IP/Hostname del nick introducido.

NICK_SERVADMIN_HELP_RENAME
	Sintaxis: 12RENAME <nick>

	Permite cambiar un nick por otro generado de manera aleatoria del tipo
	'invXXXXXX'

NICK_SERVADMIN_HELP_DROP
	Sintaxis: 12DROP [nick]
	
	Sin parámetros, cancela el registro de tu nick de la base de
	datos de 12%S.
	
	Con un parámetro, cancela el registro de ese nick en la base
	de datos.
	 
	Limitado a 4Operadores de Services.

NICK_SERVADMIN_HELP_SET
	
	4Administradores de Services pueden seleccionar la opción
	12NOEXPIRE, con la cual se puede prevenir que algunos nicks no
	expiren. Adicionalmente, los 4Administradores de Services
	pueden establecer opciones para cualquier nick sin necesidad
	de proveer una contraseña, utilizando el formato
	12SET <nick> <opción> <parámetros>.

NICK_SERVADMIN_HELP_SET_NOEXPIRE
	Sintaxis: 12SET [nick] NOEXPIRE {ON | OFF}
	
	Define si el nick dado expirara. Si se define 12ON previene
	que el nick expire. Si no se incluye un nick, entonces se
	aplica a tu propio nick.
	
	Limitado a 4Administradores de Services.

NICK_SERVADMIN_HELP_UNLINK
	
	4Administradores de Services pueden deslinkar algun nick sin
	necesidad de proveer una contraseña.

NICK_SERVADMIN_HELP_LISTLINKS
	Syntax: 12LISTLINKS <nick> [ALL]
	
	Lista los nicks los cuales estan linkados al nick dado. Si has 
	puesto 12ALL, lista todos los nicks que estan linkados.
	
	Limitado a 4Administradores de Services.

NICK_SERVADMIN_HELP_INFO
	
	4Administradores de Services pueden usar el parametro  12ALL
	con algun nick.

NICK_SERVADMIN_HELP_LIST
	Sintaxis: 12LIST <patrón> [FORBIDDEN] [NOEXPIRE] [SUSPEND]
	
	Lista todos los nicks registrados que concuerden con el
	patrón definido, en formato 12nick!usuario@host . Nicks
	con la opción 12PRIVATE activada solo serán mostrados a
	4Administradores de Services.
	
	Si has puesto la opcion 12FORBIDDEN o 12NOEXPIRE o 
	12SUSPEND solo lista los nicks, respectivamente, tienen 
	el flag 12FORBIDden o el flag 12NOEXPIRE o el flag
	12SUSPEND
	
	Estas opciones son limitados 4Administradores de Services.
	
	Ejemplos:
	
	    12LIST *!joeuser@foo.com
	        Lista todos los nicks registrados por joeuser@foo.com.

	    12LIST *Bot*!*@*
	        Lista todos los nicks registrados que contenga 12Bot
		en su nombre (no importa mayúsculas).
	
	    12LIST * NOEXPIRE
	        Lista todos los nicks registrados que no expirarán.

NICK_SERVADMIN_HELP_SENDPASS
	Sintaxis: 12SENDPASS <nick>
	
	Envia la contraseña del nick al mail asociado al nick.
	
	Limitado a 4Operadores de Services.
	
NICK_SERVADMIN_HELP_GETPASS
	Sintaxis: 12GETPASS <nick>
	
	Muestra la contraseña para el nick dado.
	
	Limitado a 4Administradores de Services.

NICK_SERVADMIN_HELP_SUSPEND
	Sintaxis: 12SUSPEND <nick> <motivo>
	
	Suspende el nick temporalmente para evitar ke sea utilizado
	el nick. Se puede reactivar el nick con el comando 
	12UNSUSPEND
	
	Limitado a 4Operadores de Services.

NICK_SERVADMIN_HELP_UNSUSPEND
	Sintaxis: 12UNSUSPEND <nick>
	 
	Reactiva un nick suspendido para que se pueda usar.
	
	Limitado a 4Operadores de Services.

NICK_SERVADMIN_HELP_FORBID
	Sintaxis: 12FORBID <nick> <motivo>
	
	Prohibe el registro y la utilización de un nick. Se puede
	cancelar utilizando el comando 12UNFORBID.
	
	Limitado a 4Administradores de Services.

NICK_SERVADMIN_HELP_UNFORBID
	Sintaxis: 12UNFORBID <nick>
	
	Levanta la prohibicion de uso de un nick para que se pueda
	usar y registrar el nick.
	
	Limitado a 4Administradores de Services.


###########################################################################
#
# Mensajes de Ayuda de ChanServ
#
###########################################################################

CHAN_HELP
	4%S habilita a registrar y controlar varios aspectos de
	los canales. 4%S puede prevenir que usuarios
	malintencionados puedan "tomar" el control limitando a quién
	se permite privilegios de operador del canal.  Los comandos
	disponibles son los siguientes.  Para utilizarlos, escriba
	12/msg %S comando. Para obtener mas información de un comando
	específico, escriba 12/msg %S HELP comando.
	
	    12IDENTIFY   Para identificarse como fundador del canal
	    12SET        Fija opciones e información del canal
	    12ACCESS     Modifica la lista de usuarios privilegiados
	    12LEVELS     Redefine los niveles de accesos
	    12AKICK      Mantiene la lista de Auto-Kick
	    12DROP       Cancela la registración de un canal
	    12INFO       Muestra información de un canal
	    12LIST       Lista los canales registrados
	    12INVITE     Te invita a un canal
	    12VOICE      Da voz en un canal
	    12DEVOICE    Quita voz en un canal
	    12OP/DEOP    Da y quita OP en un canal
	    12KICK       Kickea a un usuario en un canal
#	    12BAN        Poner un BAN en un canal
	    12UNBAN      Borra BAN's de un canal
	    12CLEAR      Reinicia los modos de un canal
	    12RESET      Resetea el canal
	    12DELACCESS  Elimina nuestro registro de un canal.

CHAN_HELP_EXPIRES
	
	Note que si un canal no es utilizado por 12%d días (es decir
	qué ningún usuario en la lista de acceso del canal entra en
	ese periodo de tiempo) será automáticamente cancelado.

CHAN_HELP_REGISTER
	Sintaxis: 12REGISTER <canal> <contraseña> <descripción>
	
	Registra un nuevo canal en la base de datos de 12%S.
	Para poder utilizar este comando, usted primero debe ser
	operador del canal que está tratando de registrar (1). La
	contraseña deberá ser utilizada con el comando 12IDENTIFY
	cuando necesite hacer modificaciones posteriores a opciones
	del canal.  El último parámetro, que 12debe ser incluido,
	es una descripción de los propósitos del canal.
	
	Cuando usted registra un canal, usted queda registrado como
	el "fundador" del canal. El fundador de un canal puede
	modificar cualquiera de las opciones del canal (excepto el
	nombre mismo del canal); 12%S le dará automáticamente
	privilegio de operador al fundador del canal cuando el/ella
	ingresen al canal.  Vea el comando 12ACCESS
	(12/msg %S HELP ACCESS) para obtener información de como
	dar privilegio de operadores a otros usuarios del canal.
	
	(1) AVISO: para poder registrar un canal, su nick debe estar
	registrado.  Si aún no lo registró, escriba
	12/msg %s HELP para obtener información de cómo hacerlo.

CHAN_HELP_IDENTIFY
	Sintaxis: 12IDENTIFY <canal> <contraseña>
	
	Este comando, lo identifica a usted ante 12%S como el
	fundador del 12canal. Muchos comandos requieren que usted
	utilice ESTE comando antes de poder utilizarlos. La
	contraseña debe ser la misma que se ingresó al momento de
	registrar el canal (comando 12REGISTER).

CHAN_HELP_DROP
	Sintaxis: 12DROP <canal> <motivo>
	
	Elimina el 12canal de la base de 12%S.  Puede ser
	utilizado únicamente por el 12fundador del canal, quien deberá
	primeramente identificarse con el comando 12IDENTIFY.

CHAN_HELP_SET
	Sintaxis: 12SET <canal> <opción> <parámetros>
	
	Habilita al fundador del canal a definir varias opciones y
	otra información. El fundador, debe identificarse vía
	comando 12IDENTIFY antes de utilizar 12SET.
	
	Opciones disponibles:
	
	    12FOUNDER     Cambia el fundador del canal
	    12PASSWORD    Cambia la contraseña del fundador
	    12SUCCESSOR   Cambia el sucesor del canal
	    12DESC        Cambia la descripción del canal
	    12URL         Asocia una URL al canal
	    12EMAIL       Asocia una dirección de correo al canal
	    12ENTRYMSG    Fija un mensaje que será mostrado a los usuarios
	                 cuando entren al canal
	    12TOPIC       Cambia el Tema del canal
	    12KEEPTOPIC   Retiene el tema (topic) cuando el canal no está en uso
	    12TOPICLOCK   El Tema SOLO podrá ser cambiado vía SET TOPIC
	    12MLOCK       Fija los modos del canal en 'ON' u 'OFF'
	    12PRIVATE     Oculta el canal (no se ve cuando se utiliza el comando LIST)
	    12RESTRICTED  Restringe el acceso al canal
	    12SECURE      Activa los rasgos de seguridad de %S
	    12SECUREVOICES Control estricto de Status de moderador
	    12SECUREOPS   Control estricto de Status de operador
	    12LEAVEOPS    Nunca deopear a usuarios excepto con el comando DEOP
	    12ISSUED      Modo DEBUG, Envia una noticia al canal sobre
	                los comandos usados
	    12STAY        %S en el canal aunque no haya nadie en el canal
	
	Escriba 12/msg %S HELP opción para obtener mas
	información de una opción en particular.

CHAN_HELP_SET_FOUNDER
	Sintaxis: 12SET <canal> FOUNDER <nick>
	
	Cambia el fundador del 12canal.  El nick dado, que será el
	nuevo fundador del canal, debe estar registrado.

CHAN_HELP_SET_SUCCESSOR
	Syntax: 12SET <canal> SUCCESSOR <nick>
	
	Cambia el sucesor del canal. Si el nick del founder expira o
	es dropeado, el canal pierde el registro, con el sucesor se
	convierte en el nuevo founder del canal y evita la perdida del
	canal. Pero si el sucesor tiene demasiados canales registrados
	(12%d), el canal será dropeado al no haber ningun sucesor.
	El nick del sucesor debe estar registrado.

CHAN_HELP_SET_PASSWORD
	Sintaxis: 12SET <canal> PASSWORD <contraseña>
	
	Se utiliza para definir la contraseña que será utilizado
	para identificarse como el fundador del canal.

CHAN_HELP_SET_DESC
	Sintaxis: 12SET <canal> DESC <descripción>
	
	Fija la descripción del canal, la cual será mostrada cuando
	se utilicen los comandos 12LIST e 12INFO. Sea breve y concreto.

CHAN_HELP_SET_URL
	Sintaxis: 12SET <canal> URL [url]
	
	Asocia una URL al 12canal, la cual será visualizada cuando
	se solicite información del canal (vía comando 12INFO).
	Habitualmente se utiliza para indicar la dirección de la
	página web del canal.  Si no se ingresa ninguna url, se
	borrará la que tenga actualmente el canal.

CHAN_HELP_SET_EMAIL
	Sintaxis: 12SET <canal> EMAIL [dirección]
	
	Asocia la dirección de correo dada al canal.  Esta dirección
	será mostrada cuando alguien solicite (vía comando 12INFO)
	información del canal. Es muy útil, por ejemplo, para que
	los usuarios tengan una vía de comunicación con el fundador
	y/u operadores del canal. Si no se proporciona una
	dirección de correo, se eliminará la que actualmente pueda
	tener el canal.

CHAN_HELP_SET_ENTRYMSG
	Sintaxis: 12SET <canal> ENTRYMSG [mensaje]
	
	Establece el mensaje que será enviado a los usuarios (vía
	/notice) cuando ellos ingresen al canal.  Si mensaje no se
	ingresa, nada será enviado a los usuarios cuando ingresen.

CHAN_HELP_SET_TOPIC
	Sintaxis: 12SET <canal> TOPIC <texto>
		
	Causa que 12%S fije el tema del canal al indicado en
	12texto. Esta orden es muy útil en conjunción con
	12SET TOPICLOCK.

CHAN_HELP_SET_KEEPTOPIC
	Sintaxis: 12SET <canal> KEEPTOPIC {ON | OFF}
	
	Habilita o deshabilita la opción de 12retención del tema para
	el canal. Cuando 12retención del tema está en ON
	(habilitada), el tema del canal será recordado por 12%S
	aún después que el último usuario se haya retirado, y se
	restaurará la próxima vez que alguien ingrese al canal.

CHAN_HELP_SET_TOPICLOCK
	Sintaxis: 12SET <canal> TOPICLOCK {ON | OFF}
	
	Habilita o deshabilita la opción para trabar el tópico del
	canal. Cuando 12trabar el tópico está en ON, 12%S no
	habilitará el cambio del tema (tópico) excepto vía 12SET
	TOPIC.

CHAN_HELP_SET_MLOCK
	Sintaxis: 12SET <canal> MLOCK <modos>
	
	Fija los distintos modos para el canal. 12%S lo habilita a
	usted para definir determinados modos del canal y que queden
	siempre habilitados (o no).  Los modos que pueden ser
	fijados son: i, k, l, m, n, p, s, y t; cada
	uno de estos modos puede estar activado (ON), desactivado
	(OFF) o no fijado.  Los distintos modos se indican
	exactamente como si se utilizase el comando 12/MODE; esto es,
	modo seguido de + indica activado (ON), modo seguido de -
	indica desactivado (OFF).
	
	4Cuidado!:  Si usted fija un modo protegido por contraseña,
	como en el segundo ejemplo, también deberá fijar la opción
	12RESTRICTED para el canal (vea 12HELP SET RESTRICTED), o
	cualquiera que ingrese al canal cuando el mismo esté vacío,
	verá la contraseña!!
	
	Ejemplos:
	
	    12SET #canal MLOCK +nt-iklps
	       Activa los modos n y t, y desactiva los modos i, k,
	       l, p, y s.  El modo m puede ser activado o desactivado.
	
	    12SET #canal MLOCK +knst-ilmp my-key
	       Activa los modos k, n, s, y t.
	       Desactiva los modos i, l, m, y p.
	       También establece "my-key" como la contraseña del canal.
	
	    12SET #canal MLOCK +
	       Elimina los modos activos; todos los modos del canal
	       pueden ser libremente activados o desactivados.

CHAN_HELP_SET_PRIVATE
	Sintaxis: 12SET <canal> PRIVATE {ON | OFF}
	
	Habilita o deshabilita la opción de 12privado para el canal.
	Cuando esta opción está activada, el comando 12/msg %S LIST no
	incluirá el canal en ninguna lista.

CHAN_HELP_SET_RESTRICTED
	Sintaxis: 12SET <canal> RESTRICTED {ON | OFF}
	
	Habilita o deshabilita la opción de 12acceso restringido para
	el canal. Cuando el acceso restringido está activo (ON),
	usuarios que normalmente no tienen privilegio de operadores
	cuando entran al canal (usuarios con nivel de acceso
	negativo y, si 12secure ops está activada, usuarios que NO
	están en la lista de acceso) serán expulsados (y se les
	prohibirá el acceso) al canal.

CHAN_HELP_SET_SECURE
	Sintaxis: 12SET <canal> SECURE {ON | OFF}
	
	Habilita o deshabilita la seguridad de 12%S para el
	canal. Cuando 12SECURE está 12ACTIVO, SOLO los usuarios que
	tienen registrado sus nicks con 12%s y se han
	IDENTIFICADO con sus respectivas contraseñas tendrán acceso
	al canal si es que figuran en la lista de 12ACCESS.

CHAN_HELP_SET_SECUREOPS
	Sintaxis: 12SET <canal> SECUREOPS {ON | OFF}
	
	Habilita o deshabilita la opción de 12operadores seguros para
	el canal. Cuando operadores seguros está activada (ON),
	usuarios que 12NO estén en la lista de usuarios del canal NO
	podrán obtener status de operadores

CHAN_HELP_SET_LEAVEOPS
	Syntax: 12SET <canal> LEAVEOPS {ON | OFF}
	
	Habilita o deshabilita la opcion de 12leave ops para el
	canal. Cuando 12leave ops está activada (ON),
	12%S no dejara nunca de-opear a usuarios en el canal
	excepto con el comando 12DEOP.

CHAN_HELP_SET_OPNOTICE
	Syntax: 12SET <canal> ISSUED {ON | OFF}
	
	Habilita o deshabilita la opcion de 12issued para el
	canal. Cuando 12issued está activada (ON),
	12%S enviará un notice al canal cuando los usuarios usen los
	comandos en el canal.

CHAN_HELP_SET_STAY
	Syntax: 12SET <canal> STAY {ON | OFF}
	 
	Habilita o deshabilita la opcion de 12stay para el
	canal. Cuando 12stay está activada (ON),
	12%S entrara en el canal y estara todo el tiempo aunque
	no haya nadie en el canal.

CHAN_HELP_ACCESS
	Sintaxis: 12ACCESS <canal> ADD <nick> <nivel>
	          12ACCESS <canal> DEL {nick | posición-numero, list}
	          12ACCESS <canal> CLEAR
	          12ACCESS <canal> LIST [mascara]
	          12ACCESS <canal> COUNT	         
	
	Mantiene la 12lista de accesos del canal. La lista de accesos
	establece que usuarios están habilitados a ser operadores
	del canal o acceder a comandos de 12%S en el canal. Se pueden
	establecer diferentes niveles de usuarios para acceder a
	diferentes privilegios; utilice 12/msg %S HELP ACCESS LEVELS
	para obtener información mas específica. Cualquier nick que
	no esté en la lista de acceso tiene nivel de usuario 0
	(cero).
	
	El comando 12ACCESS ADD agrega al 12nick con el 12nivel dado a la
	lista de acceso; si el usuario ya existe en la lista, se
	reemplazará su 12nivel. El 12nivel especificado debe ser menor
	que el que tiene el usuario que está ingresando el comando,
	y si el 12nick está actualmente en la lista, el nivel actual
	de dicho nick debe ser menor que el nivel del usuario que
	está ingresando el comando.
	
	El comando 12ACCESS DEL elimina un usuario de la lista de
	accesos. Se puede eliminar un usuario ingresando el nick o
	ingresando el número que ocupa en la lista de accesos.
	
	El comando 12ACCESS CLEAR borra todos los usuarios de la 
	lista de accesos. Sólo accesible para el founder.
	
	El comando 12ACCESS LIST muestra la lista de accesos al canal;
	se pueden utilizar caracteres comodines o mascaras, en cuyo
	caso solo las coincidencias concurrentes serán listadas.
	
	El comando 12ACCESS COUNT muestra el número de registros que
	tiene la lista de accesos del canal. Necesitas tener el nivel
	de ACC-LIST para visualizar la lista.

CHAN_HELP_ACCESS_LEVELS
	12Niveles de acceso para usuarios
	
	Por defecto, los siguientes niveles son definidos:
	
	   12Founder   Acceso FULL a las funciones de 12%S.
	                 Privilegio de operador al ingresar al canal.
	                 Note que únicamente una sola persona puede
	                 tener acceso de fundador del canal (no se
	                 puede dar acceso de founder con el comando
	                 ACCESS).
	   12    499   Acceso al comando SET, OPDEOP, INVITE, UNBAN, AKICK, ACC, MEMO;
	                 operador automático al ingresar.
	   12    450   Acceso al comando OPDEOP, INVITE, UNBAN, AKICK, ACC, MEMO;
	                 operador automático al ingresar.
	   12    300   Acceso al comando OPDEOP, INVITE, UNBAN,
	                 operador automático al ingresar.
	   12    100   Estado de VOZ automático (+v).
	   12      0   No tiene privilegios especiales; puede obtener
	                 status de operador dado por otro operador
	                 (a menos que secure-ops esté activado).
	   12     <0   No puede ser operador, ni entrar al canal.
	
	Estos niveles pueden ser cambiados, o nuevos pueden ser
	agregados utilizando el comando 12LEVELS; escriba
	12/msg %S HELP LEVELS para información.

CHAN_HELP_AKICK
	Sintaxis: 12AKICK <canal> ADD <mascara> [razón]
	          12AKICK <canal> DEL <mascara>
	          12AKICK <canal> LIST [mascara]
	          12AKICK <canal> ENFORCE
	
	Definimos AKICK como "Expulsión automática".
	Definimos BAN como "Entrada prohibida".
	
	Este comando, mantiene la lista de AKICK del canal. Si un
	usuario está en la lista de AKICK e intenta entrar al canal,
	12%S automáticamente agrega dicho usuario a la lista BAN
	del canal, y luego lo expulsa del canal.
	
	El comando 12AKICK ADD agrega un usuario a la lista de AKICK.
	Si una 12razón es dada con el comando, entonces la razón será
	utilizada por 12%S cuando expulse a dicho usuario; si
	no, el mensaje que se utilizará por defecto, es "Has sido
	baneado del canal".
	
	El comando 12AKICK DEL elimina al usuario de la lista de
	AKICK. Este comando no remueve el BAN que pueda existir en
	el canal, el cual deberá ser removido manualmente.
	
	El comando 12AKICK LIST muestra la lista de usuarios en la
	lista de AKICK. Opcionalmente, solo lista aquellos
	suministrados con la mascara dada.
	
	El comando 12AKICK ENFORCE causa que 12%S impide que los
	usuarios con mascara de AKICK en la lista de AKICK estén en 
	el canal. Es parecido a CLEAR USERS, pero que solo afecta a
	los que tienen AKICK.

CHAN_HELP_LEVELS
	Sintaxis: 12LEVELS <canal> SET <tipo> <nivel>
	          12LEVELS <canal> {DIS | DISABLE} <tipo>
	          12LEVELS <canal> LIST
	          12LEVELS <canal> RESET
	
	El comando 12LEVELS permite un control fino sobre los accesos
	numéricos utilizados por los canales.  Con este comando,
	usted puede definir los niveles requeridos para muchas de
	las funciones de 12%S. Los comandos 12SET FOUNDER,
	12SET PASSWORD, así como éste comando están siempre
	restringidos al fundador.
	
	12LEVELS SET habilita el acceso para una función o grupo de
	funciones a ser modificadas.
	
	12LEVELS DISABLE (o 12DIS en forma abreviada) deshabilita un
	privilegio automático, o deshabilita el acceso para
	cualquier otro que no sea el fundador del canal.
	
	12LEVELS LIST muestra los niveles actuales para cada función o
	grupo de funciones.
	
	12LEVELS RESET vuelve todos los niveles a sus valores por
	defecto, los mismos que cuando se registra por primera vez
	un canal (vea 12HELP ACCESS LEVELS).
	
	Para una lista de las funciones que pueden ser establecidas,
	utilice el comando 12HELP LEVELS DESC.

CHAN_HELP_LEVELS_DESC
	La siguiente lista muestra los valores/funciones por
	defecto. Note que los niveles para AUTODEOP y NOJOIN son
	los máximos niveles, mientras que todos los demás, son los
	mínimos.

CHAN_HELP_LEVELS_DESC_FORMAT
	    %-*s  %s

CHAN_HELP_INFO
	Sintaxis: 12INFO <canal> [ALL]
	
	Provee información acerca del 12canal (o informa que no está
	registrado). Lista el fundador, hora y fecha de
	registración, última vez que se utilizó, descripción, y
	modos del canal (si los hubiese). Si estas identificado como
	el founder del canal, puedes usar el parametro 12ALL, para
	ver el sucesor y el mensaje de entrada del canal.

CHAN_HELP_LIST
	Sintaxis: 12LIST <especificación>
	
	Lista todos los canales que coincidan con la especificación
	dada. Canales con la opción PRIVATE activada, no son
	listados.

CHAN_HELP_OP
	Sintaxis: 12OP <canal> <nick>
	
	Convierte en Operador al usuario indicado en el canal
	indicado. Por defecto, está limitado a quienes posean nivel
	300 o mayor en el canal.

CHAN_HELP_DEOP
	Sintaxis: 12DEOP <canal> <nick>
	
	Quita el estado de Operador al usuario indicado en el canal
	indicado. Por defecto, está limitado a quienes posean nivel
	300 o mayor en el canal.

CHAN_HELP_VOICE
	Sintaxis: 12VOICE <canal> <nick>
	 
	Convierte en moderador al usuario indicado en el canal
	indicado. Por defecto, está limitado a quienes posean nivel
	100 o mayor en el canal.

CHAN_HELP_DEVOICE
	Sintaxis: 12DEVOICE <canal> <nick>
	 
	Quita el estado de moderador al usuario indicado en el canal
	indicado. Por defecto, está limitado a quienes posean nivel
	100 o mayor en el canal.

CHAN_HELP_INVITE
	Sintaxis: 12INVITE <canal>
	
	Le dice a 12%S que lo invite a usted al canal indicado.
	Por defecto, solo funciona si su nivel de acceso en dicho
	canal es 300 o mayor.

CHAN_HELP_KICK
	Sintaxis: 12KICK <canal> <nick> [razón]
	 
	Le dice a 12%S que kickee del canal a una persona.
	Por defecto, solo funciona si su nivel de acceso en dicho
	canal es 300 o mayor.

CHAN_HELP_BAN
	Sintaxis: 12BAN <canal> <mask>
	 
	Le dice a 12%S que banee del canal a una mascara.
	Por defecto, solo funciona si su nivel de acceso en dicho
	canal es 300 o mayor.

CHAN_HELP_UNBAN
	Sintaxis: 12UNBAN <canal>
	
	Le dice a 12%S que elimine todos los accesos prohibidos
	previniendo su ingreso a dicho canal. Por defecto, solo
	funciona si su nivel de acceso en dicho canal es 300 o mayor.

CHAN_HELP_CLEAR
	Sintaxis: 12CLEAR <canal> <opcion>
	
	Le dice a 12%S que elimine ciertos parámetros del canal.
	que puede ser uno de los siguientes:

	     12MODES   Resetea todos los modos del canal (ejemplo,
	                 limpia los modos i,k,l,m,n,p,s,t).
	     12BANS    Elimina todos los accesos prohibidos al canal.
	     12OPS     Elimina (quita) el privilegio de operador a
	                 todos los operadores (modo +o) del canal.
	     12VOICES  Elimina (quita) el privilegio de "voz" (modo
	                 +v) a cualquiera del canal que lo tuviese.
	     12USERS   Elimina (Patea -kicks-) TODOS los usuarios del
	                 canal.
	     12TOPIC   Resetea el topic.
	
	Por defecto, su nivel de acceso en el canal debe ser al
	menos 499 para utilizar este comanndo.

CHAN_HELP_RESET
	Sintaxis: 12RESET <canal> <opcion>
	 
	Resetea el canal para sincronizar el canal en casos de desincroni-
	zación del canal.
	 
	Por defecto, su nivel de acceso en el canal debe ser al
	menos 499 para utilizar este comando.

CHAN_HELP_VERIFY
	Sintaxis: 12VERIFY <nick>

	Verifica si el 12nick es o no un operador/administrador de los
	servicios de la red.

CHAN_HELP_DELACCESS
	Sintaxis: 12DELACCESS <#canal>

	Nos elimina el registro que tengamos en el canal deseado, sin necesidad
	de tener que ser borrados por un operador del canal ó el fundador

CHAN_SYNTAX_DELACCESS
	DELACCESS <#canal>

CHAN_MSG_DELACCESS_SUCCESS
	Tu registro en 12%s ha sido eliminado

CHAN_MSG_DELACCESS_NEG
	No tienes registro positivo en el canal 12%s


CHAN_HELP_IRCOPS
	Sintaxis: 12IRCOPS

	Lista todos los usuarios on-line que son operadores o
	administradores de los servicios de la red. 

CHAN_SERVADMIN_HELP
	
	Los siguiente comandos están disponibles para los
	4Administradores de los Servicios:
	    
	    12REGISTER   Para registrar un canal
	    12IRCOPS     Ver los Admins/Ircops/Opers on-line
	    12VERIFY     Verificar si un usuario es Admin/Ircops/Opers o no
	    12GETPASS    Devuelve la contraseña del fundador
	    12SUSPEND    Suspende un canal
	    12UNSUSPEND  Reactiva un canal suspendido
	    12FORBID     Previene que un canal pueda ser utilizado
	    12UNFORBID   Libera la prohibicion de un canal prohibido
	    12STATUS     Devuelve el nivel de acceso actual de un
	               usuario en un canal

	Los 4Administradores de Servicios también pueden eliminar
	cualquier canal sin necesidad de identificarse vía
	contraseña, y pueden ver la lista de accesos, la lista de
	AKICK, y listar los valores de nivel (LEVELS) de cualquier
	canal.

CHAN_SERVADMIN_HELP_DROP
	Sintaxis: 12DROP <canal> <motivo>
	
	Elimina un canal de la lista de 12%S. Únicamente
	4Administradores de Servicios pueden eliminar un canal para
	lo cual ellos no necesitan identificarse.

CHAN_SERVADMIN_HELP_SET
	
	Administradores de Services también pueden definir la opción
	12NOEXPIRE, con la cual algún canal se puede prevenir que
	expire. Adicionalmente, los Administradores de Services
	pueden definir opciones para cualquier canal sin necesidad
	de identificarse con contraseña para ese canal.

CHAN_SERVADMIN_HELP_SET_NOEXPIRE
	Sintaxis: 12SET <canal> NOEXPIRE {ON | OFF}
	
	Define si el canal dado expirara. Si se define 12ON previene
	que el canal expire.

	Limitado a 4Administradores de Services.

CHAN_SERVADMIN_HELP_INFO
	
	4Operadores de Services pueden usar el parametro 12ALL con
	algun canal.

CHAN_SERVADMIN_HELP_LIST
	Sintaxis: 12LIST <especificación>
	
	Lista todos los canales que coinciden con la especificación
	dada. Los canales con la opción PRIVATE activada solo serán
	mostrados a los 4Administradores de Servicios.

CHAN_SERVADMIN_HELP_GETPASS
	Sintaxis: 12GETPASS <canal>
	
	Devuelve la contraseña del canal dado.
	
	Limitado a 4Administradores de Servicios.


CHAN_SERVADMIN_HELP_SUSPEND
	Sintaxis: 12SUSPEND <canal> <motivo>
	
	Suspende ul canal temporalmente para ke no sea utilizado.
	
	Limitado a 4Operadores de Servicios.


CHAN_SERVADMIN_HELP_UNSUSPEND
	Sintaxis: 12UNSUSPEND <canal>
	
	Reactiva un canal suspendido para que pueda ser utilizado
	
	Limitado a 4Operadores de Servicios.


CHAN_SERVADMIN_HELP_FORBID
	Sintaxis: 12FORBID <canal> <motivo>
	
	Deshabilita a cualquiera para utilizar o registrar el canal
	dado. Puede ser cancelado con el comando 12UNFORBID.
	
	Limitado a 4Administradores de Servicios.

CHAN_SERVADMIN_HELP_UNFORBID
	Sintaxis: 12UNFORBID <canal>
	
	Levanta la prohibicion de utilizar o registrar el canal
	dado.
	
	Limitado a 4Administradores de Servicios.


CHAN_SERVADMIN_HELP_STATUS
	Sintaxis: 12STATUS <canal> <nick>
	
	Devuelve el nivel de acceso actual del nick dado en el canal
	dado. La respuesta es de la forma:
	
	    12STATUS canal nick nivel-de-acceso
	
	Limitado a 4Administradores de Servicios.

###########################################################################
#
# MemoServ help messages (Mensajes de ayuda de MemoServ)
#
###########################################################################

MEMO_HELP
	4%S es una útileria que permite a usuarios de IRC
	enviar mensajes cortos a otros usuarios de IRC,
	independientemente de si están en línea o no, igualmente se
	puede enviar mensajes a canales(*). Ambos nicks (remitente
	y destinatario) deben de estar registrados, así mismo el
	canal debe de estar registrado si se desea enviar un mensaje
	a este.
	
	Comandos de 4%S incluyen:
	
	    12SEND   Envía un mensaje a un nick/canal
	    12CANCEL Cancela el ultimo mensaje enviado
	    12LIST   Lista todos tus mensajes
	    12READ   Lee un mensaje
	    12DEL    Borra un mensaje (o todos)
	    12SET    Define opciones relacionadas con mensajes
	
	Escriba 12/msg %S HELP comando para ayuda mas especifica
	de los comandos arriba mencionados.
	
	(*) Por defecto, cualquier usuario con al menos nivel 450 de
	    acceso en un canal puede leer los mensajes de ese canal.
	    Esto puede ser cambiado con el comando 12%s LEVELS.
MEMO_HELP_SEND
	Sintaxis: 12SEND <nick> <memo-texto>
	
	Envía un memo al usuario especificado.  Dicho usuario
	recibirá una noticia de que tiene un nuevo memo. Lo
	recibirá inmediatamente si se encuentra en línea, o si no al
	momento de identificarse la próxima vez que se conecte.
	
	Ejemplo: /msg %S SEND joselito Muy bueno tu trabajo de traducción! ;)

MEMO_HELP_LIST
	Sintaxis: 12LIST [NEW]
	
	Lista los memos que usted tenga actualmente.
	Si se indica el parámetro 12NEW, solo los nuevos memos (los no
	leídos) serán mostrados. Los memos no leídos están marcados
	con un "*" a la izquierda del numero de memo.

MEMO_HELP_READ
	Sintaxis: 12READ {numero | LAST}
	
	Muestra el contenido del memo indicado en numero, o el
	ultimo memo (por ejemplo, el recibido mas recientemente) si el
	parámetro 12LAST es dado EN VEZ DE 12numero.

MEMO_HELP_DEL
	Sintaxis: 12DEL {numero[-numero][,numero...] | ALL}
	
	Si un numero 12num es dado, elimina dicho memo. Usted puede
	reemplazar un único numero por un rango de números para
	eliminar múltiples memos simultáneamente, tal como se verá
	en el segundo ejemplo.
	
	Si se ingresa 12ALL se eliminaran TODOS los memos.
	
	Ejemplos:
	
	   12DEL 1
	      Elimina su primer memo.
	
	   12DEL 2-5,7-9
	      Elimina los memos 2 a 5 y 7 a 9.

MEMO_HELP_SET
	Sintaxis: 12SET <opción> <parámetros>
	
	Establece varias opciones para los memos. Las opciones
	pueden ser:
	
	    12NOTIFY      Establece CUANDO debe ser notificado de
	                    nuevos memos
	    12LIMIT       Establece el MAXIMO numero de memos que
	                    usted puede recibir
	
	Escriba 12/msg %S HELP SET opción para obtener mas
	información acerca de la opción específica.

MEMO_HELP_SET_NOTIFY
	Sintaxis: 12SET NOTIFY {ON | LOGON | NEW | OFF}
	
	Establece CUANDO debe ser notificado de nuevos memos:
	
	    12ON      Será notificado cuando se conecte, cuando quite
	                su modo /AWAY y cuando un memo le sea
	                enviado estando usted conectado.
	    12LOGON   Será notificado únicamente cuando se conecte,
			y cuando quite su modo /AWAY.
	    12NEW     Será notificado solamente cuando le sean
	                enviados.
	    12OFF     Usted no recibirá ningún tipo de notificación de
	                memos.
	
	12ON es básicamente la combinación de 12LOGON y 12NEW
	simultáneamente.

MEMO_HELP_SET_LIMIT
	Sintaxis: 12SET LIMIT [canal] <limite>
	
	Establece el máximo numero de memos que usted está
	habilitado a tener. Si establece 12limite a 0 (cero) nadie
	podrá enviarle memos a usted. De todos modos, usted no puede
	establecer 12limite mas alto que 12%d.

MEMO_HELP_INFO
	Sintaxis: 12INFO [canal]
	
	Muestra información de la cantidad de memos que usted tiene,
	cuantos de ellos no han sido leídos, y la cantidad de memos
	que usted puede recibir

MEMO_SERVADMIN_HELP_SET_LIMIT
	Sintaxis: 12SET LIMIT [usuario] {limite | NONE} [HARD]
	
	Establece el máximo número de memos que un usuario puede
	tener. Estableciendo el 12limite a 0 (cero), previene que
	dicho 12usuario pueda recibir memos; estableciéndolo a 12NONE,
	habilita al 12usuario a recibir y mantener tantos memos como
	el desee. Si usted NO ESPECIFICA usuario SU propio límite
	es el que se establece.
	mascara
	Agregando el parámetro 12HARD, previene que el usuario pueda
	cambiar dicho límite. No agregando 12HARD tiene el efecto
	opuesto, habilitando al usuario a cambiar el límite (aun si
	previamente había sido establecido con el parámetro HARD).
	
	El uso del comando 12SET LIMIT está limitado a los
	4Administradores de Servicios. Otros usuarios solo pueden
	establecer sus propios límites, no pueden remover sus
	límites, no pueden establecer límites superiores a 12%d, y no
	pueden establecer el limite 12HARD.

MEMO_SERVADMIN_HELP_INFO
	Sintaxis: 12INFO [usuario]
	
	Sin parámetros, muestra información del número de memos que
	tu tienes, cuantos están sin leer, y cuantos memos puedes
	recibir.
	
	Con el parámetro 12usuario, muestra la misma información, pero
	del 12usuario especificado. Esta opción está limitada
	únicamente a los 4Administradores de Servicios.

###########################################################################
#
# OperServ help messages (mensajes de ayuda de OperServ)
###########################################################################

OPER_HELP
	Comandos de 4%S:
	
	    12GLOBAL         Envía un mensaje a TODOS los usuarios por privmsg
	    12GLOBALN        Envía un mensaje a TODOS los usuarios por notice
	    12STATS          Muestra el estado de los servicios y de la red
	    12SERVERS        Muestra los servidores de la red
	    12PREOPER LIST   Lista todos los Pre-Operadores de los Servicios
	    12OPER LIST      Lista todos los Operadores de los Servicios
	    12CREGADMIN LIST Lista todos los Administradores de CReG
	    12COADMIN LIST   Lista todos los CoAdministradores de Servicios
	    12ADMIN LIST     Lista todos los Administradores de Servicios
	    12BOTS LIST      Lista todos los Bots cliente oficiales
	
	Comandos disponibles para Operadores y superiores:
	    12LOGONNEWS   Añade un LOGONNEWS    
	    12OPERNNEWS   Añade un OPERNNEWS
	    12MODE        Cambia los modos de un canal
	    12CLEARMODES  Quita todos los modos de un canal
	    12KICK        Patea a un usuario de un canal
	    12BLOCK       Expulsa 5 minutos a un tipejo
	    12UNGLINE     Elimina un BLOCK/GLINE
	    12GLINE       Manipula la lista de GLINEs
	    12SETTIME     Sincroniza la RED a tiempo real
	    12APODERA     MassDeop de los usuarios de un canal
	    12LIMPIA      MassKick de los usuarios de un canal
	    12KILL        Expulsa a un usuario de la red anónimamente
	 
	Comandos disponibles solo para los Administradores:
	
	    12IPVIRTUAL   Cambia la IPvirtual de un usuario en la tabla v
	    12IPVIRTUAL2  Cambia la IPvirtual de un usuario en la tabla w
	    12PREOPER     Modifica la lista de PreOperadores de Servicios
	    12OPER        Modifica la lista de Operadores de Servicios
	    12JUPE        "Jupiter" un servidor
	    12RAW         Envía un código RAW al servidor IRC
	    12SET         Establece varias opciones globales
	    12UPDATE      Fuerza un guardado de las DBs
	    12QUIT        Quita los servicios de la red
#	    12SHUTDOWN    Graba las DBs y quita los servicios
	    12RESTART     Reinicia los servicios de la red
	
	Comandos disponibles únicamente para el Root de la red:
	    12BOTS        Modifica la lista de Bots cliente oficiales (+B)
	    12CREGADMIN   Modifica la lista de Administradores de CReG
	    12COADMIN     Modifica la lista de CoAdministradores de servicios
	    12ADMIN       Modifica la lista de Administradores de Servicios
	
	4Aviso: TODOS los comandos enviados a 12%S son logueados!

OPER_HELP_GLOBAL
	Sintaxis: 12GLOBAL <mensaje>

	Habilita a los IRCops a enviar un 12mensaje a todos los
	usuarios en la red. El mensaje puede ser enviado para 12%s.

OPER_HELP_VHOST
	Sintaxis: 12IPVIRTUAL <nick> [mascara]
	
	Modifica la máscara virtual de un usuario (por tabla V) que esté +r, ó
	la elimina, en caso de que no introduzcamos [mascara]

OPER_HELP_KILL
	Sintaxis: 12KILL <nick> [mensaje]
	
	Realiza un KILL 'anónimo' a un usuario de la red. Si no se especifica
	[mensaje], se envía "Have a nice day!"

OPER_VHOST_SYNTAX
	IPVIRTUAL <nick> [máscara]

OPER_VHOST_SET
	IPvirtual para 12%s cambiada a 12%s
	

OPER_VHOST_UNSET
	IPvirtual para 12%s deshabilitada

OPER_KILL_SUCCESS
	Orden KILL enviada con éxito

OPER_HELP_STATS
	Sintaxis: 12STATS [AKILL | ALL]

	Sin opciones, muestra la cantidad de usuarios e IRCops en
	línea, el mas alto numero de usuarios en línea desde que los
	servicios se pusieron en funcionamiento, y desde cuando
	están funcionando los servicios.

	Con el parámetro 12AKILL muestra la longitud de la lista de
	AKILL y la fecha actual de expiración.
	
	El parámetro 12ALL solo está disponible para los
	4Administradores de Servicios y muestra el uso de memoria de
	los Servicios. Utilizando esta opción puede congelar los
	Servicios por un breve período de tiempo en grandes redes,
	por lo tanto no abuse de ella.
	
	12UPTIME puede ser utilizado como sinónimo de 12STATS.

OPER_HELP_OPER
	Sintaxis: 12{PREOPER|OPER} ADD <nick>
	          12{PREOPER|OPER} DEL <nick>
	          12{PREOPER|OPER} LIST
	
	Habilita a los 4Administradores de Servicios para agregar
	(ADD), borrar (DEL) o listar (LIST) usuarios de la lista de
	operadores o preoperadores. Un usuario que se encuentre en la
	lista de operadores y que se identifique ante 12%s podrá
	acceder a los comandos de Operador de Servicios.
	
	Cualquiera de los operadores de IRC (IRCops) puede utilizar
	el comando 12OPER LIST y 12PREOPEROPER LIST. Todos
	los otros comandos solo pueden 	ser utilizados por los 
	4Administradores de Servicios.

OPER_HELP_ADMIN
	Sintaxis: 12{CREGADMIN|COADMIN|ADMIN} ADD <nick>
	          12{CREGADMIN|COADMIN|ADMIN} DEL <nick>
	          12{CREGADMIN|COADMIN|ADMIN} LIST
	
	Habilita a los 4súper-usuarios de los Servicios a agregar
	(ADD), eliminar (DEL) o listar (LIST) usuarios de la lista
	de administradores, cregadmins y coadministradores. Un usuario
	que se encuentre en la lista de operadores y que se identifique
	ante 12%s podrá acceder a los comandos de Administrador de
	Servicios.
	
	Cualquiera de los operadores de IRC (IRCops) puede utilizar
	el comando 12ADMIN LIST, 12CREGADMIN LIST y
	12COADMIN LIST. Todos los otros comandos solo pueden
	ser utilizados por el 4súper-usuarios de los Servicios.
	
OPER_HELP_MODE
	Sintaxis: 12MODE <canal> <modos>
	
	Habilita a los operadores de Servicios a establecer los
	modos para cualquier canal. Los parámetros son los mismos
	que se utilizan en la forma estándar de /MODE.
	
	Solo disponible para los 4Operadores de Servicios.

OPER_HELP_CLEARMODES
	Sintaxis: 12CLEARMODES <canal> [ALL]
	
	Elimina los BAN y los modos de un canal (i,k,l,m,n,p,s,t).
	Si el parámetro 12ALL es dado, también quita los modos
	Operador y voz que haya en el canal (modos +o y +v).
	
	Solo disponible para los 4Operadores de Servicios.

OPER_HELP_KICK
	Sintaxis: 12KICK <canal> <usuario> <razón>
		
	Habilita a un IRCop a sacar a un usuario de cualquier canal.
	Los parámetros son los mismos que para un /KICK común. El
	mensaje de kick mostrará al usuario el nombre del IRCop que
	está enviando el comando. Por ejemplo:
		
	3*** Lammer ha sido kickeado del canal #mi_canal por %S (ZoLtan (Insultos))
	
	Solo disponible para los 4Operadores de Servicios.

OPER_HELP_AKILL
	Sintaxis: 12AKILL ADD [+expiracion] <mascara> <razón>
	          12AKILL DEL <mascara>
	          12AKILL LIST [mascara]
	          12AKILL VIEW [mascara]
	
	Habilita a los IRCops a manipular la lista de 12AKILL. Si un
	usuario que figura en dicha lista, intenta conectarse, los
	Servicios bloquearán su conexión y darán instrucciones a
	todos los servidores que lo soporten, que agreguen un ban
	(k-line) para la máscara que coincida con la del usuario.
	
	12AKILL ADD agrega el usuario dado (con la mascara
	usuario@host) a la lista de akill por la razón dada (que
	debe ser dada).
	
	12AKILL DEL quita el usuario dado de la lista de 12AKILL si el
	mismo está presente.
	
	12AKILL LIST muestra todos los usuarios que están actualmente
	en la lista. Si una máscara es dada, el listado estará
	limitado a las ocurrencias.
	
	12AKILL VIEW es mas explícito que 12AKILL LIST, dado que muestra
	quien agregó el 12AKILL, la fecha en que fue hecho, cuando
	expira, la máscara utilizada y la razón.
	
	Opcionalmente, un tiempo de expiración puede ser
	suministrado con el comando 12AKILL ADD. El tiempo de
	expiración PRECEDE a la máscara usuario@host y es
	especificado como un entero (esto es, sin decimales) seguido
	de: d para especificar días, h (horas), o m (minutos).
	Combinaciones (como 1h30m) no están permitidas.  Si un
	especificador de unidades no es incluido, por defecto se
	asumen DIAS (entonces, poner +30 sin nada, significa 30
	días). Para agregar un 12AKILL que NO expira, utilice +0.  Si
	la máscara de usuario comienza con +, un período de
	expiración deberá ser indicado, aún si es el mismo período
	que el asignado por defecto. El tiempo de expiración por
	defecto, se puede ver utilizando el comando STATS AKILL.
	
	La 12razón está indicada para uso de los IRCops, y NO será
	mostrada al usuario cuando se efectúe el KILL.
	
	Solo disponible para los 4Operadores de Servicios.

OPER_HELP_JUPE
	Sintaxis: 12JUPE <server>
	
	Instruye a los Servicios a crear un servidor satélite. Esto
	es, para simular un "servidor" que esté conectado a los
	Servicios los cuales previenen la conexión del servidor real
	con ese nombre. Esta simulación puede ser removida,
	utilizando un 12SQUIT estándar.

	Solo disponible para los 4Administradores de Servicios.

OPER_HELP_RAW
	Sintaxis: 12RAW <texto>
	
	Envía una cadena de caracteres directamente al servidor al
	cual los Servicios están conectados.
	Esta orden tiene un muy limitado rango de usos, y puede
	causar estragos en una red si se utiliza inadecuadamente.
	4NO USE ESTE COMANDO a menos que usted esté completamente
	seguro de lo que está haciendo!

	Solo disponible para los 4Administradores de Servicios.

OPER_HELP_SET
	Sintaxis: 12SET <opción> <modo>
	
	Establece varias de las opciones globales de los Servicios.
	Los nombres de las opciones actualmente definidas son:
	    12READONLY   Establece el modo de "solo-lectura" o
	                   "lectura-escritura"
	    12DEBUG      Activa o desactiva el modo depuración
	
	Solo disponible para los 4Administradores de Servicios.

OPER_HELP_SET_READONLY
	Sintaxis: 12SET READONLY {ON | OFF}
	
	Activa o desactiva el modo "solo-lectura". Si el modo
	"solo-lectura" está activado, los usuarios que tienen acceso
	no estarán habilitados a modificar los datos de los
	Servicios, incluyendo listas de canales, nicks, etc. IRCops
	con suficientes privilegios sobre los servicios, podrán
	modificar la lista de 12AKILL, así como eliminar (DROP) un
	canal o un nick, pero ninguno de los cambios serán grabados
	a menos que el modo "solo-lectura" se desactive después que
	finalicen los servicios, o sean restaurados.
	
	Esta opción es equivalente a la opción 12-readonly de la
	línea de comando.

OPER_HELP_SET_DEBUG
	Sintaxis: 12SET DEBUG {ON | OFF | numero}
	
	Activa o desactiva el modo depuración.  En modo depuración,
	todos los datos enviados y recibidos así como los mensajes
	de depuración, son grabados en un archivo de registro (log).
	Si numero es dado, el modo de depuración es activado, con
	nivel de depuración establecido a 12numero.
	
	Esta opción es equivalente a la opción 12-debug de la
	línea de comando.

OPER_HELP_UPDATE
	Sintaxis: 12UPDATE
	
	Causa que los Servicios actualicen las bases de datos tan
	pronto como usted envía el comando.
	
	Limitado a 4Administradores de Servicios.

OPER_HELP_QUIT
	Sintaxis: 12QUIT
	
	Causa la desactivación de los Servicios; las bases de datos
	4NO se graban. Este comando no debe utilizarse a menos que
	se sospeche de un daño en las bases de datos en-memoria y no
	deseen grabarse. Para desactivación normal de los
	Servicios, utilice el comando 12SHUTDOWN.
	
	Limitado a 4Administradores de Servicios.

OPER_HELP_SHUTDOWN
	Sintaxis: 12SHUTDOWN
	
	Causa que los servicios graben todas las bases de datos y
	luego se desactiven.
	
	Limitado a 4Administradores de Servicios.

OPER_HELP_RESTART
	Sintaxis: 12RESTART
	
	Causa que Services grabe todas las bases de datos y
	reinicialize (ejemplo: terminar y ejecutar inmediatamente el
	programa).
	
	Limitado a 4Administradores de Servicios.

OPER_HELP_ROTATELOG
	Syntax: 12ROTATELOG
	
	Renames service's log file. The current date, in the form
	YYYYMMDD, is used to generate the new filename.
	
	Limitado a 4súper-usuarios de Servicios


###########################################################################
#
# Mensajes de ayuda de CyberServ
#
###########################################################################

CYBER_HELP
	4%S es un servicio que la RED pone a disposicion de los
	CyberCafes, con el fin de proporcionar una mayor facilidad a
	dichos locales y sus multiples conexiones simultaneas que
	que tiene (Clones).
	Para utilizar los comandos teclee 12/msg %S comando.
	
	Para mayor informacion sobre un comando especifico teclee
	12/msg %S HELP comando.
	
	    12KILL       Killea a un Usuario de tu Cyber
	    12GLINE      Glinea a un Usuario de su Cyber
	    12UNBAN      Desbanea la IP de su Cyber de un canal.
	    12CLONES     Mira los clones que estan conectados de tu Cyber.
	
	4NOTA: Este servicio esta diseñado para dotar a los CyberCafes
	de una "Pseudo-protección" de estos en la RED. Su intencion
	no es la de fomentar su mal uso.
	El mal uso de 4%S llevará a la denegacion de dicho servicio
	por parte de la RED a dicho CyberCafé.

CYBER_HELP_KILL
	Sintaxis: 12KILL <nick> [motivo]
	
	Killea al usuario on-line de tu cyber.
	
	Limitado a 4Admininistradores de Cybers.

CYBER_HELP_GLINE
	Sintaxis: 12GLINE <nick> [motivo]
	
	Glinea durante 5 minutos al usuario on-line de tu cyber.
	
	Limitado a 4Administradores de Cybers.

CYBER_HELP_UNBAN
	Sintaxis: 12UNBAN <canal>
	
	Le dice a 12%S que elimine todos los accesos prohibidos
	previniendo el ingreso de tu Cyber a dicho canal.
	
	Limitado a 4Administradores de Cybers.

CYBER_HELP_CLONES
	Sintaxis: 12CLONES
	
	Te dice cuantos clones, bla bla. xDDDD

CYBER_SERVADMIN_HELP
	
	Los siguientes comandos están disponibles para los
	4Administradores de Services:
	
	    12ILINE      Añade clones a un determinado nick/host
	    12CLONES	   Información sobre clones
	    12KILLCLONES Killea a todos los usuarios de una ip
	
	Los 4Administradores de Services bla bla.

OPER_HELP_EXCEPTION
	Sintaxis: 12ILINE ADD [+expiracion]
	          12ILINE DEL {mascara | list}
	          12ILINE MOVE <numero> <posicion>
	          12ILINE LIST [mascara | list]
	          12ILINE VIEW [mascara | list]
	
	Allows Services admins to manipulate the list of hosts that
	have specific session limits - allowing certain machines,
	such as shell servers, to carry more than the default number
	of clients at a time. Once a host reaches it's session limit,
	all clients attempting to connect from that host will be
	killed. Before the user is killed, they are notified, via a
	/NOTICE from OperServ, of a source of help regarding session
	limiting. The content of this notice is a config setting.
	
	12ILINE ADD adds the given host mask to the exception list.
	Note that 12nick!user@host and 12user@host masks are invalid!
	Only real host masks, such as 12box.host.dom and 12*.host.dom,
	are allowed because sessions limiting does not take nick or
	user names into account. 12limite must be a number greater than
	or equal to zero. This determines how many sessions this host
	may carry at a time. A value of zero means the host has an
	unlimited session limit. See the 12AKILL help for details about
	the format of the optional 12expiracion parameter.
	
	12ILINE DEL borra el host de la lista de Ilines..
	
	12ILINE MOVE mueve el Iline 12numero a la 12posicion. The
	exceptions inbetween will be shifted up or down to fill the gap.
	
	12ILINE LIST y 12ILINE VIEW muestra todos los Ilines actuales
	exceptions; if the optional mask is given, the list is limited
	to those exceptions matching the mask. The difference is that
	12ILINE VIEW is more verbose, displaying the name of the
	person who added the exception, it's session limit, reason,
	host mask and the expiry date and time.
	
	Note that a connecting client will "use" the first exception
	their host matches. Large exception lists and widely matching
	exception masks are likely to degrade services' performance
	
	Limitado a 4Administradores de Servicios.

OPER_HELP_SESSION
	Sintaxis: 12CLONES LIST <número>
	          12CLONES VIEW <host>
	
	Permite a los 4Administradores de Services listar los clones en
	uso de la red.
	
	12CLONES LIST lista los hosts con hasta 12numero de clones.
	El numero debe ser un numero mayor que 1. Esto es para prevenir
	listados accidentales de hosts con un 1 solo clon.
	
	12CLONES VIEW muestra informacion detallada sobre un 12host
	especifico, incluyendo el numero de clones actual y el limite 
	máximo de clones que tiene concedidos.
	El valor del 12host no debe tener signos (! o @).
	
	Ver la ayuda de 12ILINE para más informacion sobre el limite de
	clones y como poner limites de clones especificos para ciertos hosts
	y grupos.
	
	Limitado a 4Administradores de Servicios.

OPER_HELP_KILLCLONES
	Sintaxis: 12KILLCLONES <nick>
	
	Killea a todos los users con el mismo host (clones) que
	el usuario llamado 12nick. Un Gline temporal, en la
	forma 12*@host, es añadido para prevenir que los clientes
	reconectaran inmediatamente. Un mensaje en el canal de control
	se enviara cuando sea usado el comando, con el host afectado y
	cuantos usuarios han sido killeados.
	Esto se usa para borrar muchos clones en una red.
	
	Limitado a 4Administradores de Servicies.

#######################################################################
#
# CReG SERVER
#
########################################################################

CREG_HELP
	4%S le permite realizar una petición de registro de un canal
	e introducir a éste en la fase de registro. Para poder registrar
	un canal, necesita el apoyo de12 %d personas. Los siguientes comandos
	le permiten manejar el pre-registro de un canal; para utilizarlos,
	escriba 12/msg %S comando. Para más información sobre un comando
	específico, escriba 12/msg %S HELP comando.
	
	   12REGISTRA   Realiza una petición de registro de un canal.
	   12APOYA      Muestra su apoyo al registro de un canal.
	   12CANCELA    Cancela una petición de registro de un canal.
	   12ESTADO     Permite consultar el estado de un canal.
	   12APOYOS     Lista los apoyos recibidos por un canal o
	              los registros/apoyos emitidos por un usuario.
	 
	4AVISO: 12%S realiza únicamente funciones de pre-registro de
	un canal, siendo cada petición revisada por la administración
	de la red. La red se reserva el derecho a denegar el registro a
	cualquier canal si sus usuarios se comportan indebidamente o si
	el canal no se ajusta a las normas de registro para los canales
	disponibles en la página de la red.


CREG_HELP_REGISTRA
	Sintaxis: REGISTRA <canal> <contraseña> <descripción>
	
	Realiza una petición de registro de un nuevo canal.
	
	El canal que intenta registrar no puede estar ya previamente
	registrado con el bot 4%s. Consulte el comando 12ESTADO para
	obtener información sobre como conocer el estado actual de un
	canal. La contraseña que proporcione servirá para que pueda
	identificarse como fundador del canal sin estar con su nick
	habitual. El último parámetro debe ser una descripción delión
	 propósito del canal.
	
	Para realizar una petición de registro, el nick desde el que la
	ha hecho debe estar registrado, y si el registro es aceptado, ese
	nick pasará a ser el fundador del canal.
	
	Para que una petición de registro sea válida, necesita el apoyo de
	al menos 12%s personas en un plazo de siete días, no contando
	como apoyos válidos distintos nicks registrados con el mismo email.
	Si en ese plazo no recibe los apoyos necesarios, el canal expirará.
	y será necesaria la intervenci¢n de un administrador de la red para
	que se pueda proceder a una petición de registro de nuevo. Consulte
	el comando APOYA (12/msg %S HELP APOYA) para obtener
	información sobre como se puede apoyar el registro de un canal.
	
	4NOTA: Sólo puede realizar UNA operación sobre canales al día. Esto
	es, únicamente puede utilizar el comando 12REGISTRA una vez cada
	veinticuatro horas. Antes de utilizarlo, piense sabiamente y
	no lo u en vano, ya que no existen excepciones posibles a esta
	limitación.


CREG_HELP_APOYA
	Sintaxis: 12APOYA <canal> [contraseña]
	
	Muestra su apoyo a la petición de registro de un canal.
	
	El canal que introduzca no puede estar ya registrado, sino que debe
	estar en un estado de registro pendiente en el bot 4%S. (Debe haber
	utilizado el comando REGISTRA para iniciar este proceso) Consulte el
	comando ESTADO (/msg CReG AYUDA estado) para obtener información
	sobre como conocer el estado de un canal.
	
	La contraseña que introduzca no debe ser la contraseña del canal,
	sino la contraseña que 4%S le proporcionará cuando ejecute
	12/msg %S APOYA #canal, con el propósito de verificar su aprobación
	al registro del canal.
	
	Para que un apoyo sea válido, su nick debe estar registrado y no
	puede haber apoyado ese canal previamente desde ese nico desde
	cualquier otro nick que posea. Además, no se permiten apyos que
	provengan de nicks registrados con emails anónimos.
	Tenga en cuenta que para evitar abusos por parte de los usuarios,
	el máximo número de apoyos que un canal puede recibir es uno por
	minuto. De esta manera, 4%S le informará de si su apoyo ha sido
	procesado o de si por lo contrario, ha de esperar a que haya
	transcurrido cierto periodo de tiempo.
	
	4NOTA: Sólo puede realizar UNA operación sobre canales al día. Esto
	es, únicamente puede utilizar el comando 12APOYA una vez cada
	veinticuatro horas. Antes de utilizarlo, piense sabiamente y
	no lo use en vano, ya que no existen excepciones posibles a esta
	limitación.


CREG_HELP_CANCELA
	Sintaxis: 12CANCELA <canal>
	
	Cancela la peticion de un registro de canal. Solo el founder (el que
	ha hecho la peticion de registro) puede ejecutar este comando.



CREG_HELP_ESTADO
	Sintaxis: 12ESTADO <canal>
	
	Muestra el estado actual del canal que especifique.
	
	Los posibles estados de un canal son los siguientes:
	
	    12PENDIENTE    El canal no ha obtenido todavía el numero
	                 necesario de apoyos para que pueda ser procesada
	                 la petición.
	    12ENREGISTRO   La petición de registro del canal está siendo
	                 revisada por la administración de canales de la
	                 red.
	    12ACEPTADO     La petición de registro ha sido aprobada.
	    12DENEGADO     La petición de registro ha sido denegada.
	    12REGISTRADO   El canal ya está actualmente registrado.
	    12DROPADO      El registro del canal ha sido elminado por la
	                 administración de la red.


CREG_HELP_APOYOS
	Sintaxis: 12APOYOS {canal|nick|email}
	
	Lista los nicks de las personas que apoyan la petición de
	registro de un canal si especificamos el nombre de un canal, o
	los registros y/o apoyos que ha emitido un/os usuario si
	especificamos un nick/email.
	
	4NOTA: Sólo es posible listar los nicks de las personas que apoyan
	el canal si se está utilizando el nick que env¡o la petición de
	registro (el futuro fundador del canal), y solo es posible
	listar los apoyos/registros del nick que estamos utilizando.


CREG_SERVADMIN_HELP
	Los siguientes comandos están disponibles para los Operadores
	de services:
	
	    INFO      Devuelve información ampliada sobre un canal.
	    LISTA     Lista los canales pendientes, denegados, prohibidos,
	              expirados, enregistro, aceptados, suspendidos,
	              comerciales o dropados.
	    ACEPTA    Acepta la petici¢n de registro de un canal
	              y lo registra en el bot CHaN. (Admins)
	    DENIEGA   Deniega la petición de registro de un canal. (Admins)
	    DROP      Borra el registro de un canal registrado.
	    FUERZA    Acepta un canal que no ha recibido los apoyos
	
	Operadores de services pueden también listar los apoyos que ha
	recibido un canal sin ser el fundador del mismo y los apoyos y
	registros de cualquier usuario.

SERVICES_CREDITS
	Services es copyright (c) 1996-2000 de Andrew Church
	<12achurch@achurch.orgt>.
	
	Estos bots han sido reprogramados y adaptados originalmente para
	la red 3Upworld.Org, usando la base de iRCServices 4.3.3, por
	Toni García Núñez - zoltan <12zolty@zolty.net>.
	
	Por petición popular, estos bots han sido liberados bajo la
	licencia GNU-GPL. Puedes bajar la última versión de estos bots
	o más informacion en la siguiente web:
	12http://irc.zolty.net/upworld.html
	 
	----------------------------------------------------------------
	 
	Esta es una modificación de los Upworld de Toni García creada
	para interactuar con la Base de Datos Distribuida del IRCd de
	la red iRC-Hispano. Modificación esctrita por David Martín Díaz
	[x] - <equis@fuckmicrosoft.com>
	 
	----------------------------------------------------------------
	 
	La modificación de los HispanoBDD para adaptarlos a DarkU ha
	sido realizada por Javier Fdez. Viña (a.k.a ZipBreake). Si desea
	más información, colaborar con su desarrollo o hacer sugerencias
	pongase en contacto con el autor en: 
	<12javier@jfv.es>, 12http://jfv.es/
	 
######## NOTA IMPORTANTE ##########
######## NO BORRES NI CAMBIES LAS LINEAS DE LOS CREDITOS
######## PUEDES AÑADIR LINEAS, PERO NO CAMBIAR O BORRAR
	Más informacion de los Services Originales (Licencia GNU):
	Las ultimas versiones de los Services Originales pueden ser obtenidas de:
	La web de los Services es: <12http://ender.shadowfire.org/ircservices/>
	El FTP de los services es: <12ftp://ender.shadowfire.org/pub/ircservices/>

BDD_HELP
	12%S permite llevar a cabo tareas varias de mantenimiento
	y administración del sistema de Base de Datos Distribuida del
	IRCD, así como simplificar su manejo. Permite llevar a cabo
	operaciones no integradas en algún otro service, como
	pueden ser:
		
		12COMPACTAR     Compacta las tablas de la BDD para ahorrar CPU y disco
		12REGENERAR     Regenera manualmente una clave de cifrado para IPs
#		12ULINE         Añade una U-line global a un servidor de bots
#		12NODOP9        Añade un nodo protocolo P09 para su funcionamiento
#		12NOMBRERED     Modifica el nombre de la red (lo del STATUS)
#		12DOMINIORED    Modifica el dominio de la red.
		12ACTUALIZAR    Re-lee información del servidor para tener una sincronización
		12TOCAR         Toca manualmente una tabla.
	
	4ATENCIÓN! El uso de cualquiera de estos comandos puede resultar peligroso
	para la integridad de las tablas en la Base de Datos Distribuida!

BDD_HELP_COMPACTAR
	Sintaxis: 12COMPACTAR
	
	Compacta tablas de la base de datos para ahorrar CPU y espacio en disco.
	Las tablas de toda la red son reducidas según sea conveniente.

BDD_HELP_REGENERAR
	Sintaxis: 12REGENERAR
	
	Genera una clave de cifrado aleatoria de alta calidad (pero de 64 bits)
	para el sistema de cifrado de IPs virtuales. Esta clave 4NO debería ser conocida
	por nadie más que no sea el que la genera.
	
BDD_HELP_ACTUALIZAR
	Sintaxis: 12ACTUALIZAR
	
	Actualiza los contadores internos de los services en caso de un posible "desfase"
	a causa de algún "raw" o algún otro nodo. Normalmente, nunca es necesario usar esto,
	ya que se producen sincronizaciones cada vez que se accede a la BDD.

BDD_HELP_TOCAR
	Sintaxis: 12TOCAR <tabla> [clave]
	
	'Toca' manualmente una tabla de la Base de Datos Distribuida, tratando
	la información introducida y actualizando los contadores.

BDD_REG_SUCCESS
	Clave de cifrado regenerada con éxito, usando: 12%s

BDD_COMPACT
	Tablas compactadas

BDD_TOCAR_SYNTAX
	12TOCAR <tabla> <clave> [valor]

BDD_ERR_TABLE
	Tabla errónea

BDD_SEQ_OK
	Secuencia completa

