
%define s_prefix /opt/kernel-source-%s_platform

Name: kernel-source-%s_platform
Version: 1.0
Release: skt1
Summary: Linux kernel sources with additional changes for DM8168's based boards

License: GPL
Group: Development/Kernel

BuildArch: noarch

AutoReq: no
AutoProv: no
AutoReqProv: no
Packager: Pavel Nakonechny <pavel.nakonechny@skitlab.ru>

Source: kernel-source-%s_platform-%version.tar
Patch: %name-%version.patch

%description
%summary

%prep
%setup -n %name-%version
%patch -p1

%build

%install
rm -rf .gear

mkdir -p %buildroot%s_prefix

tar --owner=root --group=root --mode=u+w,go-w,go+rX -cjf %buildroot%s_prefix/kernel-source-%s_platform.tar.bz2 .

%files
/opt/kernel-source-%s_platform

%define __os_install_post %{nil}

%changelog
* Wed Jan 09 2013 Pavel Nakonechny <pavel.nakonechny@skitlab.ru> 1.0-skt1
- initial build
