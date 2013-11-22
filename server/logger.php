<?
    /**
     * 로그 기록
     * @author Niney
     * 2010.11.10
     *
     */
    class CLog {
       
        private static $isexe = true; // 작동 유무
       
        public static function isexe($bool) {
            if ($bool == true) {
                CLog::$isexe = true;
            } else {
                CLog::$isexe = false;
            }
        }
       
        public static function init($filename = "lprolog.log") {
            // 초기화
            //$file = fopen("{$_SERVER["DOCUMENT_ROOT"]}/{$filename}", a);
			$file = fopen("{$filename}", "a");
            return $file;
        }
        public static function write($arr_str, $filename = 'logerror.log') {
           
            if(CLog::$isexe) {                 
                // 초기화
                $file = CLog::init($filename);
                if($file == false)
                    return;
               
                $log_string = '';
               
                $log_string .= "============================================================="."\r\n";
                $log_string .= date("Y/m/d H:i:s")."\t";
                $log_string .= gethostbyaddr($_SERVER["REMOTE_ADDR"])."\r\n";
                for($i = 0 ; $i < count($arr_str); $i++) {
                    $log_string .= key($arr_str);
                    $log_string .= " : ";
                    $log_string .= current($arr_str)."\r\n";
                    next($arr_str);
                }
                $log_string .= "============================================================="."\r\n";
           
                flock($file, LOCK_EX);
                fputs($file, $log_string);
                flock($file, LOCK_UN);
       
                fclose($file);
            }
        }
    }
?>