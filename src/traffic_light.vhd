-------------------------------------------------------------------------------
-- Title : Traffic lights controller
-- Design : vhdl_test
-- Author : Dr.J.S.Parab
Note: Traffic lights controller based on state machine
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.STD_LOGIC_unsigned.all;

entity TLC is
    port (clk: in STD_LOGIC;
        clr: in STD_LOGIC;
        ledlights: out STD_LOGIC_VECTOR(5 downto 0)
    );
end TLC;

architecture trafficlight of TLC is
    type stateTC_type is (s0, s1, s2, s3, s4, s5);
    signal stateTC: state_type;
    signal count1: STD_LOGIC_VECTOR(3 downto 0);
    constant SEC15: STD_LOGIC_VECTOR(3 downto 0) := "1111";
    constant SEC3: STD_LOGIC_VECTOR(3 downto 0) := "0011";
    begin
        process(clk, clr) --clk asynchronous
        begin
            if clr = '1' then
                stateTC <= s0;
                count1 <= X"0"; --hex 0000 (nibble)
            elsif clock'event and clock = '1' then
                case stateTC is
                    when s0 =>
                        if count1 < SEC15 then
                            state <= s0;
                            count1 <= count1 + 1;
                        else
                            state <= s1;
                            count1 <= X"0";
                        end if;
                    when s1 =>
                        if count1 < SEC3 then
                            state <= s1;
                            count1 <= count1 + 1;
                        else
                            state <= s2;
                            count1 <= X"0";
                        end if;
                    when s2 =>
                        if count1 < SEC3 then
                            state <= s3;
                            count1 <= count1 + 1;
                        else
                            state <= s3;
                            count1 <= X"0";
                        end if;
                    when s3 =>
                        if count1 < SEC15 then
                            state <= s3;
                            count1 <= count1 + 1;
                        else
                            state <= s4;
                            count1 <= X"0";
                        end if;
                    when s4 =>
                        if count1 < SEC3 then
                            state <= s4;
                            count1 <= count + 1;
                        else
                            state <= s5;
                            count1 <= X"0";
                        end if;
                    when s5 =>
                        if count1 < SEC3 then
                            state <= s5;
                            count1 <= count1 + 1;
                        else
                            state <= s0;
                            count1 <= X"0";
                        end if;
                    when others =>
                        state <= s0;
                end case;
            end if;
        end process;
        C2: process(stateTC)
            begin
                case stateTC is
                    when s0 => ledlights <= "100001";
                    when s1 => ledlights <= "100010";
                    when s2 => ledlights <= "100100";
                    when s3 => ledlights <= "001100";
                    when s4 => ledlights <= "010100";
                    when s5 => ledlights <= "100100";
                    when others => ledlights <= "100001";
                end case;
            end process;
end trafficlight;