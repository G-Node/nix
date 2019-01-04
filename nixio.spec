Name:		nixio
Version:	1.5.0
Release:	1%{?dist}
Summary:	IO-libray for nix data files

License:	BSD
URL:		https://www.g-node.org/nix
Source0:	https://github.com/G-Node/nix/archive/%{version}.tar.gz

BuildRequires:	gcc-c++, cmake, boost-devel, hdf5-devel, cppunit-devel

%description
Neuroscience information exchange - data model for annotated (neuroscience)
data. For more information visit https://github.com/G-Node/nix

%package devel
Summary: nixio development files
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}
Requires: hdf5-devel, boost-devel

%description devel
nixio development headers and libraries.

%prep
%setup -q -n nix-%{version}

%build
mkdir -p build
cd build
%cmake .. \
       -DCMAKE_INSTALL_PREFIX=/usr \
       -DCMAKE_BUILD_TYPE=RelWithDebInfo
make %{?_smp_mflags}

%install
cd build
make install DESTDIR=%{buildroot}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%license LICENSE LICENSE.h5py
%doc README.md CONTRIBUTING.md
%{_bindir}/nixio-tool
%{_libdir}/libnixio.so.1*

%files devel
%{_includedir}/nixio-1.0/nix.hpp
%{_includedir}/nixio-1.0/nix/
%{_libdir}/libnixio.so
%{_libdir}/pkgconfig/nixio-1.0.pc



%changelog
* Thu Nov 01 2018 Jan Grewe <jan.grewe@g-node.org> - 1.4.0-1
- version bump to 1.4.3


* Fri Oct 20 2017 Jan Grewe <jan.grewe@g-node.org> - 1.4.1-1
- New upstream release

* Mon Sep 25 2017 Christian Kellner <gicmo@gnome.org> - 1.4.0-1
- New upstream release

* Fri Jan 20 2017 Chrisian Kellner <gicmo@gnome.org>
- Initial revision
