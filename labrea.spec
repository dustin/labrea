Name:           labrea
Version:        0.1
Release:        1
Summary:        Scripting Other People's Programs

Group:          Development
License:        MIT
URL:            https://github.com/dustin/labrea/wiki
Source:         labrea-%{version}.tgz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  python

%description
La Brea allows you to inject scripts into an existing application without
recompiling.

%prep
%setup -q

%build
%configure

make %{?_smp_mflags}

%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot} PREFIX=%{_prefix} LIBS=%{_libexecdir}

# Don't install the .la-files
find $RPM_BUILD_ROOT -type f -name "*.la" -exec rm -f {} ';'

# Default configs

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
%doc README.markdown

%{_bindir}/labrea
%{_libexecdir}/labrea.so
%{_datadir}/labrea-init.lua
%{_datadir}/labrea/examples/*lua
%attr (0755, root, bin) %{_libexecdir}/labrea.so
%attr (0644, root, bin) %{_datadir}/labrea-init.lua
%attr (0644, root, bin) %{_datadir}/labrea/examples/*.lua
%dir %attr (0755, root, bin) %{_libexecdir}
%dir %attr (0755, root, bin) %{_datadir}

%changelog
* Tue Dec  7 2010 Dustin Sallings <dustin@spy.net>
- Initial spec file created
