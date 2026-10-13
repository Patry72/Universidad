-- VHDL Entity for BCDCounter

LIBRARY ieee ;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;

ENTITY BCDCounter IS
   GENERIC( 
      prop_delay : time := 10 ns
   );
   PORT( 
      cnten    : IN     std_logic  ;
      clear    : IN     std_logic  ;
      clk      : IN     std_logic  ;
      load     : IN     std_logic  ;
      dat_in   : IN     std_logic_vector (3 DOWNTO 0) ;
      carry_in : IN     std_logic_vector (3 DOWNTO 0) ;
      count    : OUT    std_logic_vector (3 DOWNTO 0) ;
      zero     : OUT    std_logic 
   );

END BCDCounter ;

-- VHDL Architecture for BCDCounter

LIBRARY ieee ;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;

architecture spec of BCDCounter is
  
begin

  vhdl_bcd_countv : PROCESS (clk)   

  VARIABLE int_count : std_logic_vector(3 DOWNTO 0) := "0000" ;
  BEGIN
    IF RISING_EDGE(clk) THEN
      zero <= '1' ;
      IF clear = '1' THEN
        int_count := "0000" ;
      ELSIF load = '1' THEN
        int_count := dat_in ;
      ELSIF cnten = '0' THEN
        IF (int_count = "0000") AND (carry_in /= "0000") THEN int_count := "1001" ;
        ELSE
          int_count := unsigned(int_count) - 1 ;
          END IF;
      ELSE
        int_count := int_count ;
      END IF;
   END IF;
   zero <= (int_count(3) OR int_count(2)) OR (int_count(1) OR
														int_count(0));
   count <= int_count AFTER prop_delay ;
  END PROCESS vhdl_bcd_countv ;

end spec;
