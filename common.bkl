<?xml version="1.0" ?>
<makefile>
	<include file="presets/simple.bkl"/>

	<set var="DBLDOLLAR">$(DOLLAR)$(DOLLAR)</set>
	<set var="ZLIB">yes</set>

	<set var="THREAD_TYPE">single</set>
	<if cond="FORMAT in ['msvc6prj', 'mingw']">
		<set var="THREAD_TYPE">multi</set>
	</if>
		
    <option name="BUILD">
        <values>debug,release</values>
        <values-description>Debug,Release</values-description>
        <default-value>debug</default-value>
        <description>
            Type of compiled binaries
        </description>
    </option>

    <set var="DEBUGINFO">
        <if cond="BUILD=='debug'">on</if>
        <if cond="BUILD=='release'">off</if>
    </set>
</makefile>
