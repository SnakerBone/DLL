package xyz.snaker.natives;

import java.io.File;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.function.Function;

import xyz.snaker.snkr4j.SimpleLogger;
import xyz.snaker.snkr4j.SnakerLogger;

/**
 * Created by SnakerBone on 4/11/2023
 **/
public class SnakerNatives
{
    public static final SnakerLogger LOGGER = new SimpleLogger(SnakerNatives.class, true);
    public static Function<Boolean, String> libraryName = ext -> ext ? "snkr.dll" : "snkr";

    static {
        initialize();
    }

    private static void initialize()
    {
        ClassLoader loader = SnakerNatives.class.getClassLoader();
        URL resource = loader.getResource("xyz/snaker/natives");

        if (resource != null) {
            String name = libraryName.apply(true);

            String sourcePath = "src\\main\\java\\xyz\\snaker\\natives\\" + name;
            String destPath = new File(resource.getPath()).getAbsolutePath();

            Path source = Paths.get("").resolve(sourcePath).toAbsolutePath();
            Path dest = Paths.get(destPath, name).toAbsolutePath();

            copyAndLoad(source, dest);
        }
    }

    private static void copyAndLoad(Path source, Path dest)
    {
        try {
            Files.copy(source, dest, StandardCopyOption.REPLACE_EXISTING);
            System.load(dest.toString());
        } catch (Exception e) {
            LOGGER.errorf("Could not load libraries for SnakerNatives: []", e.getMessage());
            return;
        }

        LOGGER.infof("Loaded native libraries for SnakerNatives");
    }

    /**
     * Forces the current windows system to "blue screen" by calling an internal debug crash function provided in <a href="https://en.wikipedia.org/wiki/Microsoft_Windows_library_files#NTDLL.DLL">NtDll.dll</a>
     *
     * @throws UnsatisfiedLinkError If an error occurs during execution
     **/
    public native void forceCrashWindows();

    /**
     * Assigns admin privileges without needing permission from the current user/super user
     *
     * @return The result of the JNI call
     * @throws UnsatisfiedLinkError If an error occurs during execution
     **/
    public native int assignElevationNoPrompt();

    /**
     * Raises an internal error on the current windows system
     * <p>
     * {@link #assignElevationNoPrompt()} must be called prior for this to do anything as admin privileges/elevation is required
     *
     * @return The result of the JNI call
     * @throws UnsatisfiedLinkError If an error occurs during execution
     **/
    public native int raiseInternalError();
}
